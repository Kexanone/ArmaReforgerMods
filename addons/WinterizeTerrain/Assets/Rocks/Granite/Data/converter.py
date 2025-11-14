from pathlib import Path

for path in Path().rglob('*.emat'):
    output = ''

    with path.open('r') as stream:
        output = stream.readline()
        output += ' GlobalLayerMode Manual\n}'

    with path.open('w') as stream:
        stream.write(output)
