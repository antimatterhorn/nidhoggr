import optparse

def CommandLineArguments(**kwargs):
    option_list = []
    for key, value in kwargs.items():
        option = optparse.make_option(f"--{key}", type="float", default=value)
        option_list.append(option)
        globals()[key] = value
    parser = optparse.OptionParser()
    parser.add_options(option_list)

