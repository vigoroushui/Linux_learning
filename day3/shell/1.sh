read dir
mkdir ${dir}
cp -rf /etc/*.conf${dir}
ls -l /etc/* |grep ^d |wc -1>etcdir.txt
