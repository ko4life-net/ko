# Knight Online Scripting

Scripting project for Knight Online for the purpose of writing simple and portable scripts to help managing this project better.

Note that you do not have to install stuff by yourself (unless you prefer), because the config.ps1 in the root dir of the project will setup a portable python installation for you.

Some useful command below:
```sh
# assuming you're inside the script dir, even though not needed
cd script

# Install requirements
virtualenv venv
& venv/Scripts/activate.ps1
pip install -r requirements.txt
pip install .

# Test that all works
ko --help

# or
python kocmd/ko.py --help

# add `--editable` if you want to run the shorter `ko` command whilst editing the scripts:
pip install --editable .

# Run some useful command
ko --help
ko build --config debug --project game --project server
ko db --action import
ko format

# format your code before submitting a PR
autopep8 -i . --exclude venv
```
