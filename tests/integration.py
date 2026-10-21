"""Run the actual C++ binary and verify accounting, modes and scaling boundaries."""
from pathlib import Path
import subprocess, tempfile

root=Path(__file__).resolve().parents[1]
base=(root/'config.txt').read_text()
def run(changes=None, mode='--quiet', good=True):
    values=dict(line.split('=',1) for line in base.splitlines() if line and not line.startswith('#'))
    values.update(changes or {})
    with tempfile.TemporaryDirectory() as tmp:
        cfg=Path(tmp)/'config.txt'; log=Path(tmp)/'run.log'
        values['log_file']=str(log)
        cfg.write_text('\n'.join(f'{k}={v}' for k,v in values.items()))
        result=subprocess.run([str(root/'loadbalancer'),str(cfg),mode],capture_output=True,text=True)
        assert (result.returncode==0)==good, result.stderr
        if not good: return
        assert log.read_text()==result.stdout
        end=result.stdout.split('=== END SUMMARY ===\n')[1]
        d=dict(x.split(': ',1) for x in end.splitlines() if ': ' in x)
        assert int(d['Generated requests (including rejected)'])==sum(int(d[k]) for k in ['Rejected requests','Completed requests','Queue size','Busy servers (including draining)'])
        assert int(d['Assigned requests'])==int(d['Completed requests'])+int(d['Busy servers (including draining)'])
        return d,result.stdout

d,quiet=run()
assert d['Elapsed clock cycles']=='100000' and '[cycle' not in quiet
assert 'Queue size: 2000\n' in quiet.split('=== END SUMMARY ===')[0]
assert int(d['Server additions'])>0 and int(d['Server retirements'])>0
assert int(d['Rejected requests'])>0
assert run()[1]==quiet
d,verbose=run({'cycles':'20000'},'--verbose')
assert 'SCALE UP' in verbose and 'SCALE DOWN' in verbose and 'BLOCK' in verbose
d,_=run({'servers':'1','initial_per_server':'0','arrival_probability':'0','cycles':'100'})
assert d['Active servers (accepting work)']=='1' and d['Completed requests']=='0'
d,_=run({'servers':'1','max_servers':'1','initial_per_server':'100','cycles':'300','arrival_probability':'1','burst_min':'5','burst_max':'5'})
assert int(d['Capacity-limit events'])>0
# All newly generated addresses blocked; no initialization requested.
d,_=run({'initial_per_server':'0','blocked_start':'0.0.0.0','blocked_end':'255.255.255.255','cycles':'10','arrival_probability':'1'})
assert d['Accepted requests']=='0' and int(d['Rejected requests'])>0
for bad in [{'cooldown':'0'},{'arrival_probability':'nan'},{'servers':'0'},{'min_time':'101'},{'seed':'abc'},{'unknown':'1'}]:
    run(bad,good=False)
print('Integration checks passed: required run, determinism, logging, scaling, firewall, bounds')
