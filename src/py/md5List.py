import hashlib
import os
import os.path

rootdir = "."

def getFileMd5(filename):
	if not os.path.isfile(filename):
		return 
	myhash = hashlib.md5()
	f = open(filename, 'rb')
	while True:
		b = f.read(8096)
		if not b:
			break
		myhash.update(b)
	f.close()
	return myhash.hexdigest()

def getFilesMd5(curentRoot):
	for parent, dirnames, filenames in os.walk(curentRoot):
		for filename in filenames:
		#	print("parent is :" + parent)
		#	print("filename is : " + filename)
			filePath = os.path.join(parent, filename)
			print(getFileMd5(filePath), filePath)
		#print("the full name of the file is : " + os.path.join(parent, filename))

		#for dirname in dirnames:
		#	print("parent is : " + parent)
		#	print("dirname is" + dirname)

getFilesMd5(".")
