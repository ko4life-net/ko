
def build_project(**args):
  print(f"Building {args['config']} projects (cores: {args['cores']}): {list(args['project'])}")
  print(args)
