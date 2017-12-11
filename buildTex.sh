if [[ $# -eq 1 ]]; then
	./functor $1 -t
else
	./functor -t
fi
[[ $? -ne 0 ]] && exit 1
cd tex
latexmk -pdf > /dev/null

