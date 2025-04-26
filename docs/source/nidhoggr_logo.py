from docutils import nodes
from docutils.parsers.rst import Directive

class NidhoggrLogoDirective(Directive):
    has_content = False

    def run(self):
        literal = nodes.literal_block(
            text=(
                "     _   _ _                  \n"
                " ___|_|_| | |_ ___ ___ ___ ___\n"
                "|   | | . |   | . | . | . |  _|\n"
                "|_|_|_|___|_|_|___|_  |_  |_|\n"
                "      v0.8.0      |___|___|\n"
            )
        )
        literal['language'] = 'text'  # tell Sphinx it's plain text
        return [literal]

def setup(app):
    app.add_directive("nidhoggr-logo", NidhoggrLogoDirective)
