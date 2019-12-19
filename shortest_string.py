#
# Copyright (c) 2019 David Davis. All rights reserved.
#
# python 3.7 compatible
#

from collections import Counter
from collections import defaultdict


"""
Input: string = “this is a test string”, pattern = “tist”
Output: Minimum window is “t stri”

Input: string = “geeksforgeeks”, pattern = “ork”
Output: Minimum window is “ksfor”
"""


def main():
    while True:
        input_string = input("string: ")
        if not input_string:
            break
        alphabet = input("alphabet: ")
        if not alphabet:
            break

        result = getMinSubstr(input_string, alphabet)
        print("result: {}\n".format(result))


def makeCountDict(string):
    return Counter(string)


def makeIndexDict(string, alphabet):
    result = defaultdict(list)
    for i in range(0, len(string)):
        if string[i] in alphabet:
            result[string[i]].append(i)
    else:
        return result


def getSubstringIndexes(indexDict, countDict, begin, end):
    indexes = []

    for k, v in countDict.items():
        arr = indexDict.get(k, [])
        arr = list(filter(lambda index: index >= begin and index <= end, arr))
        if len(arr) < v:
            #print("getSubstringIndexes: {} begin {} end {} k {} v {}".format(arr, begin, end, k, v))
            return False, None, None
        else:
            indexes.extend(arr[0:v])
    else:
        #print("getSubstringIndexes: {} begin {} end {}".format(indexes, begin, end))
        return True, min(indexes), max(indexes)


def getMinSubstr(string, alphabet):
    indexes = []
    indexDict = makeIndexDict(string, alphabet)
    countDict = makeCountDict(alphabet)
    begin = 0
    end = len(string) - 1
    endIndex = end

    hasAll, begin, end = getSubstringIndexes(indexDict, countDict, begin, end)
    if not hasAll:
        return ""
    else:
        indexes.append((begin, end))
    
    while True:
        hasAll, begin, end = getSubstringIndexes(indexDict, countDict, begin + 1, endIndex)
        if not hasAll:
            break
        else:
            indexes.append((begin, end))

    #print(indexes)
    lengths = list(map(lambda x: x[1] - x[0], indexes))
    shortest = lengths.index(min(lengths))
    begin, end = indexes[shortest]
    return string[begin : end + 1]

main()
