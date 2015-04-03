###morse-bst

This small project includes a Morse dictionary module, and a sample command-line program using it.

The dictionary is implemented using a binary search tree. The tree is built from an input stream containing arbitrary character-codeword combinations. The input is line oriented and should be organized as follows: `A .-` (single one byte character followed by a space and the binary codeword for the character and a newline)

The sample program provides a way to use the international standard for convenience.
The `--help` output:
```
Usage:
    morse [OPTION] [DICTIONARY] [INFILE] [OUTFILE]
Options:
    -e, --encode    Encodes INFILE into OUTFILE.
                    Default behavior.
    -d, --decode    Decodes INFILE into OUTFILE.
    -o, --output    Accept one filename as OUTFILE, read from standard input.
                    Normally, if OUTFILE is omitted, the program writes to standard output.
    -s, --standard  Use the ITU standardized, International Morse Code.
                    With this option, DICTIONARY is not needed.
    -h, --help      Display this help and exit.
```