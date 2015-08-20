Nonsense generator! Yet another of the very same kind.

I am actually all for "putting things out there" it is only *potentially* dumb!

    lua bin.lua file [sentence_cnt] [sentence_length]

Sentence length is optional, it sees the end of sentences as a thing.

### NOTE:
* Could do `cat $FILE | tr '\n' ' ' | tr '.' '\n' | lua bin.lua /dev/stdin`
  because it sees lines as sentences.(and no coherence between sentences.)

* I find it surprising that this simple approach works.
