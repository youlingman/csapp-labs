mov 0x804c1e4,%eax		# set cookie as the return value
lea 0x28(%esp),%ebp 		# restore the ebp for testn
push $0x08048c93		# push the return location
ret				# return to the testn
