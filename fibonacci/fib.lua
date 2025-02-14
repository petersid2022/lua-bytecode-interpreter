function fib(n)
	if n == 1 or n == 2 then
		return 1
	end

	return fib(n - 1) + fib(n - 2)
end

for i=1,40 do
	print(fib(i))
end
