{

}
unit crm;

uses io, strings;

const Bar: enum = [

];

const Foo: struct = [

];

const Being: trait(Humanoid)
	begin
		private:
			var name: char[20] = 'Unknown';
			var age:  int(8);
			var nums: int(8)[3] = [1, 2, 3];
			const looks: struct = [
				hair:   string,
				eyes:   string,
				height: double
			];
		public:
			var
			const speak: function(): void
				begin
					write(format('My name is %s', this.name);
				end;
	end;

const Humanoid: interface()
	begin
		const speak: function();
	end;


const Person: class(Being)
	begin
		// Single line comment

		private:
			static const constant: string;
			static var variable: string;
			var name: string;
			var age: int(8);
			var friends: Person[];

		protected:

		public:
			const __create:function(name:string(255), age:int(1))
				begin
					this.name = name;
					this.age  = age;
				end;

			const __toString: function(): string
				begin
					return this.name
				end;

			const befriend: function(friend: Person): Person
				begin
					this.friends.add(friend);
				end;

			const speak: function(...name: string): void
				begin


				end;

			var do: function(): void
				begin

				end;
	end;


{
	Link dependencies
}
uses io, str;


const main: function(): int
	begin

		{
			Primitive types
		}
		var a: int;        // declare as integer (default 32-bit)
		var b: int(64);    // declare as a large integer (64-bit);
		var c: char;       // declare as a character (default 32-bit, UTF-8)
		var d: char(8);    // declare as a character (8-bit, ASCII)
		var e: float;      // declare as a float (default 32-bit)
		var f: float(16);  // declare a 16-bit float, half-precision
		var g: bool;       // declare a boolean
		var h: bool(8);	   // declare an 8-bit bitmask, h[0] = first bit, h[1] = second bit
		var i: int[10];    // declare an array of 10 32-bit integers
		var j: int(64)[5]; // declare an array of 5 64-bit integers

		{
			Get the size, in bytes, using sizeof
		}
		io.writeLn('The size of a is %d'.format(sizeof(a)));

		{
			Object types
		}
		var k: vector;           // declare an array, defaults to int
		var l: vector<int(64)>;  // declare an array of 64-bit integers
		var m: string;           // declare a string, similar to vector<char>
		var n: string<char(16)>; // declare a UTF-16 string
		var o: vector<string>;   // declare a vector of strings

		k = [1, 2, 3, 4];

		for var x in k
			then
				io.writeLn('Item at position %d is %d'.format(x, k[x]));
			end;

		a = k.sum();
		m = 'Numbers: ' ~ k.join(', ') ~ ' add up to ' ~ a;

		k.each(function(num: int, idx: int): void
			begin
				io.writeLn(num);
			end
		);

		o = k.map(function(num: int): string
			begin
				return num: string;
			end
		);

		m.lower();
		m.upper();
		m.split(':');
		m.slice(0, 50);
		m.replace(', ', ' - ');







		var person := new Person('Matthew', 40);
		var steve  := new Person('Steve', 27);

		var callback := function(): void uses (person)
			begin

			end;

		for var i in [1, 2, 3]
			write(i);

		for var i := 1; i <= 10; i++
			then

			end;

		if typeof(person) == object and classof(person) == Person
			then

			end;

		if foo.age > 50 and true
			then

			end
		else
			then
				steve.befriend(person);
			end;

		person.speak();

		return 1;
	end;

