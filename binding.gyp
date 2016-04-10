{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "main.cc" ],
      "libraries": [
        "-lleveldb"
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
