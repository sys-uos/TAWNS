# script to generate images in different sizes
PNG_FULLPATH="$1"
DIR=$2

# repalce . with whitespace
PNG=${PNG_FULLPATH/.png/}

convert $PNG_FULLPATH -resize 16x12 "../../images/${DIR}/${PNG}_vs.png"
convert $PNG_FULLPATH -resize 24x18 "../../images/${DIR}/${PNG}_s.png"
convert $PNG_FULLPATH -resize 60x45 "../../images/${DIR}/${PNG}_l.png"
convert $PNG_FULLPATH -resize 100x74 "../../images/${DIR}/${PNG}_vl.png"
convert $PNG_FULLPATH -resize 40x30 "../../images/${DIR}/${PNG}.png"

