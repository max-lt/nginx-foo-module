# nginx-foo-module
Boilerplate for nginx module

## Development env
### Copy nginx source code into working directory
```bash
# copy nginx soucre code into dev/src (for editor completion)
make setup-dev-headers
```

### VSCodeconfig
Set .vscode/c_cpp_properties.json
```json
{
  "configurations": [
    {
      "name": "Linux",
      "includePath": [
        "${workspaceFolder}/src",
        "${workspaceFolder}/dev/src",
        "${workspaceFolder}/dev/src/core",
        "${workspaceFolder}/dev/src/http",
        "${workspaceFolder}/dev/src/http/modules",
        "${workspaceFolder}/dev/src/mail",
        "${workspaceFolder}/dev/src/event",
        "${workspaceFolder}/dev/src/stream",
        "${workspaceFolder}/dev/src/os/unix"
      ],
      "defines": [],
      "intelliSenseMode": "clang-x64",
      "browse": {
        "limitSymbolsToIncludedHeaders": true,
        "databaseFilename": ""
      },
      "compilerPath": "/usr/bin/clang",
      "cStandard": "c11",
      "cppStandard": "c++17"
    }
  ],
  "version": 4
}
```

## Build image
```bash
make docker-image
```

## Run image
```bash
make docker-run
```
