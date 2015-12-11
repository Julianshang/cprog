obj-m += netlinkTest.o

all:
	make -C /usr/src/kernels/2.6.32-573.8.1.el6.x86_64 SUBDIRS=$(PWD) modules
#清除make后生成的文件
clean:
	make -C /usr/src/kernels/2.6.32-573.8.1.el6.x86_64 SUBDIRS=$(PWD) clean
