# Will McCall
July 29, 2021

## Crawler

The set of programs for crawler intend to take as input a starting URL, and crawl webpages linked to that URL, fetching and storing the whitespace removed HTML content of each of those pages in a directory for later usage. 

## Usage

to use crawler, make sure you have run `make` in the common library (../common), then run `make` to build crawler executable, which can be used in the format ./crawler URL outputDirectory Depth[0-10].
For some representative test cases you can also run `make test`, and to check for memory leaks consider prefacing the executable command with `myvalgrind`