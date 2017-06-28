import copy
# import hashlib

def leftRotate(num, shiftNum):
    return ((num << shiftNum) | (num >> (32 - shiftNum))) & 0xffffffff


def SHA1(message: bytes):
    # Initialize variable
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0
    mask = 0xffffffff
    messageLen = len(message) * 8
    message += b'\x80'
    while(len(message) % 64 != 56):
        message += b'\x00'
    message += messageLen.to_bytes(8, "big")
    assert (len(message) % 64 == 0)
    chunks = []
    copyList = copy.deepcopy(message)
    for i in range(len(message) // 64):
        chunks.append(copyList[: 64])
        copyList = copyList[64:]

    for chunk in chunks:
        w = [0] * 80
        copyChunk = copy.deepcopy(chunk)
        for i in range(16):
            w[i] = int.from_bytes(copyChunk[: 4], "big")
            copyChunk = copyChunk[4:]
        # Extend the sixteen 32-bit words into eighty 32-bits
        for i in range(16, 80):
            w[i] = leftRotate((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1)

        a = h0
        b = h1
        c = h2
        d = h3
        e = h4

        for i in range(80):
            if 0 <= i <= 19:
                f = d ^ (b & (c ^ d))
                k = 0x5A827999
            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0x6ED9EBA1
            elif 40 <= i <= 59:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC
            elif 60 <= i <= 79:
                f = b ^ c ^ d
                k = 0xCA62C1D6

            temp = (leftRotate(a, 5) + f + e + k + w[i]) & mask
            e = d
            d = c
            c = leftRotate(b, 30)
            b = a
            a = temp

        h0 = (h0 + a) & mask
        h1 = (h1 + b) & mask
        h2 = (h2 + c) & mask
        h3 = (h3 + d) & mask
        h4 = (h4 + e) & mask

    # print('%08x%08x%08x%08x%08x' % (h0, h1, h2, h3, h4))
    h = [h0, h1, h2, h3, h4]
    return (b''.join(sub.to_bytes(4, "big") for sub in h))

# SHA1(str.encode("test"))
# print(hashlib.sha1(str.encode("test")).digest())
