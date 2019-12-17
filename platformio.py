import json
import os.path
Import("env")

env.Append(CPPDEFINES=[
    "FASTLED_INTERNAL",
])

if os.path.isfile('devices.json'):
    with open('devices.json') as json_file:
        data = json.load(json_file)
        if env['UPLOAD_PROTOCOL'] == 'espota':
            env.Replace(UPLOAD_PORT=data[env['PIOPLATFORM']]['ip'])
            if data[env['PIOPLATFORM']]['pass']:
                env.Replace(UPLOAD_FLAGS="$UPLOAD_FLAGS --auth " + data[env['PIOPLATFORM']]['pass'])
        else:
            env.Replace(UPLOAD_PORT=data[env['PIOPLATFORM']]['serial'])


if "uploadfs" in BUILD_TARGETS:
    env.Execute("npm run build")
