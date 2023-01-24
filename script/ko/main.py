import os
import click

from ko.build import build_entry
from ko.db import db_entry
from ko.util import util_entry


class CaseInsensitiveChoice(click.Choice):
  def convert(self, value, param, ctx):
    return super().convert(value.lower(), param, ctx)


@click.group()
def cli():
  pass


@cli.command()
@click.option('--config', '-c', type=CaseInsensitiveChoice(['release', 'debug']), default='release', help='Build configuration mode')
@click.option('--project', '-p', type=CaseInsensitiveChoice(['all', 'game', 'server', 'tool']),
              default=['all'], multiple=True, help='Multiple projects to build')
@click.option('--cores', '-j', type=click.INT, default=os.cpu_count(), help='CPU cores count for building')
def build(**args):
  build_entry(**args)


@cli.command()
@click.option('--action', '-a', type=CaseInsensitiveChoice(['import', 'export']), required=True, help='Import or export database')
def db(**args):
  db_entry(**args)


@cli.command()
@click.option('--dir', '-d', type=click.STRING, help='Path to a directory to apply code formatting')
def format(**args):
  util_entry(**args)


if __name__ == '__main__':
  cli()
