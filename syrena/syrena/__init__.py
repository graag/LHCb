#!/bin/env python
"""
Set of ganga helper functions.

Available functions:
    syrena.data_filter_site - get from dataset only files at selected site
    syrena.job_download_output - copy DiracFiles of selected job from grid to local directory
    syrena.job_get_output_lfn - get LFNs of selected jobs DiracFiles
    syrena.job_resubmit_failed - resubmit all failed subjobs for selected job
"""

def data_filter_site(data, sites=('CERN-RDST','CERN-USER'), verbose=False):
    """
    Filter data set retaining only LFNs with replicas at specified site.

    Requires working dirac:
    SetupLHCbDirac
    SetupGanga

    Arguments:
    data - LHCbDataset to filter
    site - List of SE site names. If equal None filter out LFNs without proper replica.

    Return: filtered LHCbDataset

    Usage: data2 = LHCbDataset(syrena.data_filter_site(data))
    """
    from GangaLHCb.Lib.LHCbDataset import LHCbDataset
    from tempfile import NamedTemporaryFile
    import subprocess
    import re
    import os

    _tmp_file = NamedTemporaryFile(delete=False)
    _tmp_name = _tmp_file.name
    _data_list = "\n".join([_f.name for _f in data])
    _tmp_file.write(_data_list)
    _tmp_file.close()

    # Lets use external command as it is much faster
    _out = LHCbDataset()
    _input = ["dirac-dms-lfn-replicas","-",_tmp_name]
    _result = subprocess.check_output(_input).splitlines()
    if verbose:
        print _result
    _current = None
    _msg = None
    _relfn = re.compile(r"\s+(/lhcb.*?) :(.*)")
    _resite = re.compile(r"\s+([A-Z0-9\-]+) :.*")
    for _l in _result:
        _mlfn = _relfn.search(_l)
        if _mlfn:
            _current = _mlfn.group(1)
            _msg = _mlfn.group(2)
            if sites is None and _msg != " No allowed replica found":
                _out.extend(("lfn:%s" % _current,))
        elif sites is not None:
            _msite = _resite.search(_l)
            if _msite:
                _site = _msite.group(1)
                if _site in sites:
                    _out.extend(("lfn:%s" % _current,))
    os.unlink(_tmp_name)
    return _out

def job_resubmit_failed(job):
    """
    Resubmit subjobs in "failed" state.

    Arguments:
    job - the job object which subjobs should be resubmitted.
    """
    for _sj in job.subjobs:
        if _sj.status == "failed":
            _sj.resubmit()

def job_download_output(job, output_dir, pattern="*.root", skip=[], force=False):
    """
    Copy from the grid to specified directory all DiracFiles for completed
    subjobs.

    Arguments:
    job - the job object to get the output for
    output_dir - directory where to store the results. Will be created if missing.
    pattern - pattern of grid file names to select
    skip - list with subjob ids to skip
    force - do not skip failed jobs if true
    """
    import os
    missing = []
    found = False
    for _sj in job.subjobs:
        found = False
        if _sj.id in skip:
            missing.append(_sj.id)
            continue
        if _sj.status != "completed" or force:
            missing.append(_sj.id)
            continue
        _sdir = os.path.join(output_dir, str(_sj.id))
        os.makedirs(_sdir)
        for _f in _sj.outputfiles.get(pattern):
            if _f.lfn:
                found = True
                _f.localDir = _sdir
                _f.get()
        if not found:
            missing.append(_sj.id)
    print "Jobs with missing output:\n%s" % missing

def job_get_output_lfn(job, pattern="*.dst", skip=[], force=False):
    """
    Get LFNs of job output grid files.

    Arguments:
    job - the job object to get the output for
    pattern - pattern of grid file names to select
    skip - list with subjob ids to skip
    force - do not skip failed jobs if true

    Return: LHCbDataset

    Usage: data = LHCbDataset(syrena.job_get_output_lfn(job))
    """
    missing = []
    found = False
    from GangaLHCb.Lib.LHCbDataset import LHCbDataset
    _d = LHCbDataset()
    for _sj in job.subjobs:
        found = False
        if _sj.id in skip:
            missing.append(_sj.id)
            continue
        if _sj.status == 'completed' or force:
            _files = _sj.outputfiles.get(pattern)
            for _f in _files:
                if _f.lfn:
                    found = True
                    _d.files.append('lfn:' + _f.lfn)
        if not found:
            missing.append(_sj.id)
    print "Jobs with missing output:\n%s" % missing
    return _d
