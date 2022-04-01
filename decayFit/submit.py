#!/usr/bin/env python

import os
import sys
import stat
import json
import argparse
import errno
from subprocess import call
from jinja2 import Template

script_path = os.path.dirname(os.path.realpath(__file__))

# Read commandline parameters
parser = argparse.ArgumentParser(description='Run mass fits splitting data into categories.')
parser.add_argument('--job', '-j', required=True,
        help='Job name to use.')
parser.add_argument('--config', '-c', required=True,
        help='Run config file. Describes input data and category splitting.')
parser.add_argument('--fit', '-f', required=True,
        help='Fit config file. Describes PDFs and selection.')

args = parser.parse_args()

job_id = args.job
config = args.config
input_name = args.fit
base_dir = os.path.splitext(os.path.basename(input_name))[0]
pwd = os.getcwd()

# Read run config
with open(args.config) as config_file:
    config = json.load(config_file)
    config["data"] = os.path.abspath(config["data"])

# Read templates
with open(os.path.join(script_path, "run.sh")) as run_file:
    run_template = Template(run_file.read())
with open(input_name) as data_file:
    template = Template(data_file.read())

# os.mkdir(base_dir)

# Generate class scripts and run jobs
for win_name, win_value in config["windows"].items():
    if not win_value[0]:
        continue
    for class_name, class_cut in config["classes"].items():
        opts = {}
        run_dir = "%s_%s_%s" % (base_dir, win_name, class_name)
        try:
            os.mkdir(run_dir)
        except OSError as e:
            if e.errno == errno.EEXIST:
                print "Fit %s already exists - skipping." % run_dir
                continue
            else:
                raise e
        opts["mass_min"] = win_value[1]
        opts["mass_max"] = win_value[2]
        opts["data"] = config["data"]
        opts["class_cut"] = class_cut
        opts["tag"] = class_name
        opts["name"] = "%s_%s_%s" % (job_id, win_name, class_name)
        run_name = os.path.join(run_dir, "run.sh")
        with open(run_name, 'w') as run_out:
            run_out.write(run_template.render(opts))
        st = os.stat(run_name)
        os.chmod(run_name, st.st_mode | stat.S_IEXEC)
        with open(os.path.join(run_dir, "Fit.json"), 'w') as json_out:
            json_out.write(template.render(opts))
        os.chdir(run_dir)
        print run_dir
        call(["qsub", "./run.sh"])
        os.chdir(pwd)

