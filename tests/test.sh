#! /bin/sh 

#to use with command `make test` only


if [[ $1 == 'invalidFormat' ]]; then 
	
	index=1;
	errors=0;
	total=`ls tests/testInvalidFormat* | wc -l | awk '{$1=$1};1'`
	for file in `ls tests/testInvalidFormat*`; do

		./tp1 $file &> /dev/null

		if [[ $? -ne 0 ]]; then 
			echo '.'
		else 
			echo '=== TEST FILE' $file '('$index'/'$total') === FAILED ==='

			errors=$((errors + 1));
		fi

		index=$((index + 1));

	done
		
	if [[ $errors -ne 0 ]]; then 

		echo '=== SUMMARY :' $errors 'TESTS FAILED ===';
	else 
		echo '=== SUMMARY : OK ===';
	fi 

elif [[ $1 == 'validFormat' ]]; then 

	index=1;
	errors=0;
	total=`ls tests/testValidFormat* | wc -l | awk '{$1=$1};1'`
	for file in `ls tests/testValidFormat*`; do

		./tp1 $file &> /dev/null
		result=$?
		if [[ result -eq 0 ]]; then 
			echo '.'
		else 
			echo '=== TEST FILE' $file '('$index'/'$total') === FAILED ==='
			echo "Expected 0 got ${result}";
			errors=$((errors + 1));
		fi

		index=$((index + 1));

	done
		
	if [[ $errors -ne 0 ]]; then 

		echo '=== SUMMARY :' $errors 'TESTS FAILED ===';
	else 
		echo '=== SUMMARY : OK ===';
	fi 

fi 

