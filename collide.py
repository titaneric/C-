import sha1
import uuid
import sys
# "".join(format(ord(c), "x") for c in s) # char to hex ascii
# bytearray.fromhex("7061756c").decode() # hex ascii to string
lastbyte = 4

times = 100000
plain_list = [uuid.uuid4().hex for i in range(times)]

hash_list = [sha1.SHA1(str.encode(code))[-lastbyte:] for code in plain_list]
# duplicate = {plain_list[i]: sha for i, sha in enumerate(hash_list) if hash_list.count(sha) > 1}

duplicate = dict()
for i, h in enumerate(hash_list):
    if hash_list.count(h) > 1:
        if h not in duplicate:
            duplicate[h] = [plain_list[i]]
        else:
            duplicate[h].append(plain_list[i])

for sha, plain in duplicate.items():
    print(sha, ":")
    print(",".join(p for p in plain))
    print()

