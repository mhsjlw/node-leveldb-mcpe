{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "main.cc" ],
      "libraries": [
        '../leveldb-mcpe/out-static/libleveldb.a'
      ],
      "include_dirs" : [
        'leveldb-mcpe/include/',
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
