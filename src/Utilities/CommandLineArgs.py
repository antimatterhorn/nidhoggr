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
            option = optparse.make_option(f"--{key}", action="store_true" if not value else "store_false", default=value)
        elif isinstance(value, int):
            option = optparse.make_option(f"--{key}", type="int", default=value)
        elif isinstance(value, float):
            option = optparse.make_option(f"--{key}", type="float", default=value)
        else:
            option = optparse.make_option(f"--{key}", type="string", default=value)
        
        option_list.append(option)

    # Create parser and add options
    parser = optparse.OptionParser()
    parser.add_options(option_list)

    # Parse the arguments
    options, _ = parser.parse_args()
    parsed_args = vars(options)

    # Inject parsed variables into the calling function's local scope
    frame = inspect.currentframe().f_back
    frame.f_locals.update(parsed_args)

    return parsed_args
