mkdir src-utf8
cd src
for file in `ls`; do iconv -f UTF-16 -t UTF-8 ${file} > ../src-utf8/${file};echo "converted src/${file}"; done
echo "converted files in src to src-utf8"
