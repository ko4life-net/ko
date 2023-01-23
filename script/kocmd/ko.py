import os
import click


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
def build(project, config, cores):
  print(f"Building {config} projects (cores: {cores}): {list(project)}")


@cli.command()
@click.option('--action', '-a', type=CaseInsensitiveChoice(['import', 'export']), required=True, help='Import or export database')
def db(action):
  print(f"Database action: {action}")


@cli.command()
@click.option('--dir', '-d', type=click.STRING, help='Path to a directory to apply code formatting')
def format(dir):
  print(f"Using clang-format to format code in the following directory: {dir}")


if __name__ == '__main__':
  cli()
