#!/bin/sh
set -u
: ${AWK:=awk}
PORTFMT="${PWD}/portfmt"
status=0
ROOT="${PWD}"

cd "${ROOT}/tests/format"
: ${TESTS:=*.in}
for test in ${TESTS}; do
	t=${test%*.in}
	${PORTFMT} -t < ${t}.in > ${t}.actual
	out=$(diff -L ${t}.expected -L ${t}.actual -u ${t}.expected ${t}.actual)
	if [ $? -ne 0 ]; then
		echo "${t}#1"
		echo "${out}"
		echo
		status=1
		continue
	fi
	${PORTFMT} -t < ${t}.expected > ${t}.actual2
	out=$(diff -L ${t}.expected -L ${t}.actual -u ${t}.expected ${t}.actual2)
	if [ $? -ne 0 ]; then
		echo "${t}#2"
		echo "${out}"
		echo
		status=1
	fi
done
rm -f *.actual *.actual2

exit ${status}
