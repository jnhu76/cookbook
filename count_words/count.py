import argparse
import collections

parser = argparse.ArgumentParser(prog="wordsText")
parser.add_argument('filename', default="words.txt", type=str)

counts = collections.Counter()
# counts = {}

def count_words(filename):

    with open(filename, "r") as f:
        while True:
            line = f.readline()
            if not line:
                break
            words = line.lower().split()
            if not words:
                continue
            counts.update(words)
            # for word in words:
                # counts[word] = counts.get(word, 0) + 1

    pairs = sorted(counts.items(), key=lambda kv: kv[1], reverse=True)
    # for word, count in pairs:
        # print(word, count)
    for word, count in counts.most_common():
        print(word, count)


if __name__ == "__main__":
    args = parser.parse_args()
    count_words(args.filename)

# python3 count.py kjvbible.txt  0.17s user 0.02s system 99% cpu 0.185 total
# python3 count.py kjvbible.txt  0.16s user 0.02s system 99% cpu 0.177 total