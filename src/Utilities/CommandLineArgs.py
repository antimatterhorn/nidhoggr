import optparse
import inspect

def CommandLineArguments(**kwargs):
    """
    Parses command-line arguments and automatically injects them into the caller's local scope.

    Args:
        **kwargs: Default values for command-line arguments.

    Returns:
        dict: A dictionary with parsed command-line arguments.
    """
    option_list = []

    # Infer types and create options dynamically
    for key, value in kwargs.items():
        if isinstance(value, bool):
            # Treat booleans as strings and convert manually later
            option = optparse.make_option(f"--{key}", type="string", default=str(value))
        elif isinstance(value, int):
            option = optparse.make_option(f"--{key}", type="int", default=value)
        elif isinstance(value, float):
            option = optparse.make_option(f"--{key}", type="float", default=value)
        elif isinstance(value, tuple):
            option = optparse.make_option(f"--{key}", type="string", default=str(value))
        else:
            option = optparse.make_option(f"--{key}", type="string", default=value)

        option_list.append(option)

    parser = optparse.OptionParser()
    parser.add_options(option_list)

    options, _ = parser.parse_args()
    parsed_args = vars(options)

    # Manually convert booleans and tuples
    for key, value in parsed_args.items():
        if isinstance(kwargs.get(key), bool):
            parsed_args[key] = value.lower() in ("true", "1", "yes", "on")
        elif isinstance(kwargs.get(key), tuple):
            try:
                parsed_args[key] = eval(value)
            except:
                parsed_args[key] = kwargs[key]  # fallback

    # Inject into caller's local scope
    frame = inspect.currentframe().f_back
    frame.f_locals.update(parsed_args)

    return parsed_args
