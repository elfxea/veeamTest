# Python 3.8
#
# Usage: <path_to_script> --file <path_to_checksum_file> --dir <path_to_checking_directory>
# To test this code using files from /test, use "<path_to_script> --file check.sum --dir test"

import argparse
import hashlib
import codecs
import os.path

# Getting command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("--file")
parser.add_argument("--dir")

argv = parser.parse_args()

checksum_file = argv.file
checking_directory = argv.dir


def get_checksum(path, algorithm): # A function to get [algorithm] hashcode of file located at [path]
    with codecs.open(path, 'r', "utf_8_sig", errors="ignore") as file:
        if algorithm == "md5":
            hash = hashlib.md5()
        elif algorithm == "sha1":
            hash = hashlib.sha1()
        elif algorithm == "sha256":
            hash = hashlib.sha256()
        while True:
            data = file.read(16384)
            if not data:
                break
            hash.update(data.encode("utf-8"))
        return hash.hexdigest()


if __name__ == "__main__":
    with codecs.open(checksum_file, "r", "utf_8_sig", errors="ignore") as checksum_file:
        for line in checksum_file:
            current_file, encoding, expected_hash = list(map(str, line.split()))
            print(current_file, end=" ")

            if os.path.exists(checking_directory + '\\' + current_file): # checking if file exists
                actual_hash = get_checksum(checking_directory + '\\' + current_file, encoding) # getting hashcode
                if actual_hash == expected_hash:
                    print("OK")
                else:
                    print("FAIL")
            else:
                print("NOT FOUND")
    exit(0)
