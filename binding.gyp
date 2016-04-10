{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "main.cc" ],
      "libraries": [
        '-lleveldb', '-L../leveldb-mcpe/out-shared/'
      ],
      "include_dirs" : [
        'leveldb-mcpe/include/',
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
