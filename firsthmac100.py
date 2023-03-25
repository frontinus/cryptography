import sys
import hashlib
import hmac

if len(sys.argv) != 3:
    print("Usage: python hmac_sha256.py <file1> <file2>")
    sys.exit()

key = b"keykeykeykeykeykey"
file1 = sys.argv[1]
file2 = sys.argv[2]

with open(file1, 'rb') as f1, open(file2, 'rb') as f2:
    data = f1.read() + f2.read()

hmac_digest = hmac.new(key, data, hashlib.sha256).hexdigest()

print("CRYPTO23{" + hmac_digest + "}")
