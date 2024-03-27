# Pascal Inspired New-Talk (PINT)

PINT (like beer) is a **hypothetical** object-oriented programming language.  Its syntax is heavily inspired by Pascal, so much so that you can obtain syntax highlighting generally by just enabling any `.pint` file to use a Pascal language file.  Unlike Pascal, PINT has a number of more modern features which aim to reduce repetition and enhance availability of run-time information out of the box.

PINT is neither compiled, nor interpreted, because it's not real **yet**.  However, the ultimate aim is to have a compiled language with flexibility in choice of dynamic (less efficient) or type-safe (more efficient) features and data structures.

## Basic Usage

### Hello World

```pascal
unit main;

uses io;

register main: function(): void
begin
	io.writeLn('Hello, World!');
end
```

### Declaring Variables

Variables can be declared separately or declared during assignment.  The universal declaration format is the following.

```erlang
var <name>:[<type>][([<size>])][[[<length>]]];
```

When a variable is declared, the signature of the declaration says a lot about the variable.  There are three main concepts to cover here:

#### Type vs. Size vs. Length

Values have both a type and a size.  This is contrast to other languages where the size is often part of type.  What the size refers to depends on the type, for numbers it refers to its bit-length, for characters and strings it refers to the UTF code unit size.  Sizes should not be thought of as the amount of memory consumed.  Valid sizes are always multiple of 8.

Length only applies to arrays.  The length is the logical length of the thing.  For an array of characters (aka: a string), this would be the length of the string.

#### Explicit vs. Implicit

Both the type and size of a variable can be explicitly or implicitly declared.  If these are explicitly declared, then you would not be able to initialize the value with an incompatible type/size.  If these are implicitly declared, then the type/size are determined upon initialization, but are immutable.

#### Fixed vs. Dynamic

Both the type and size of a variable can also be fixed or dynamic.  If a type of a variable is fixed, whether the type was determined explicitly (at declaration) or implicitly (at initialization) it cannot be changed.  A dynamic type, by contrast, means that the type of the variable can change.  The same logic applies to size.

#### Examples

Explicit fixed type and explicit fixed size:

```pascal
var x: integer(8);
```

Explicit fixed type and implicit fixed size and 

```pascal
var x: integer();
```

Explicit fixed type and implicit dynamic size:

```pascal
var x: integer;
```

Implicit fixed type and explicit fixed size:

```pascal
var x:(8);
```

Implicit fixed type and implicit fixed size:

```pascal
var x:();
```

Implicit fixed type and implicit dynamic size:

```pascal
var x:;
```

Implicit dynamic type and implicit dynamic size:

```pascal
var x;
```

Any of the above declaration styles can turned into an array by appending `[]`:

```pascal
var x: integer(8)[];
```

You can fix the length of an array by adding a number inside the brackets:

```pascal
var x: integer(8)[5];
```

Strings are just dynamic length arrays of characters.  The following two declaration are equivalent:

```pascal
var x: char(8)[];
var y: string(8);
```

Accordingly, if you want a fixed length string (as opposed to an array of strings), you need to use something like the following:

```pascal
var x: char(8)[5]
```

The following would created a fixed length array of strings (themselves of dynamic length):

````pascal
var x: string(8)[5];
````

Similarly a boolean value declared as follows would result in an array of booleans with a size of one and an array length equivalent to the specified size:

```pascal
var x: boolean(8);
```

### Types and Default Sizes

Whenever a variable is declared without explicit size, or when a constant of a certain type is used in the code, the default sizes are as follows:

| Keyword    | Default Size | Description                                                  |
| ---------- | ------------ | ------------------------------------------------------------ |
| `integer`  | 32           | Signed integer value.  Default for any `var x:integer` or constant (in code), e.g. `5`. |
| `cardinal` | 32           | Unsigned integer value.  Default for any `var x:cardinal`.  There is no way to specify a constant (in code) as a cardinal. |
| `real`     | 32           | Signed float values.  Default for any `var x:real` or constant (in code), e.g. `5.5`. |
| `char`     | 8            | Character value, size indicates multi-byte unit size. Characters are UTF-8.  Default for any `var x:char` or constant single character string (in code), e.g. `'c'`. |
| `string`   | 8            | String value, size indicates multi-byte unit size. Strings are UTF-8.  Default for any `var x:string` or constant multi-character string (in code), e.g. `'string`'. |
| `boolean`  | 1            | Boolean value, size > 1 indicates a bitmask, equivalent to `var x:boolean(1)[<size>]` |

### Assignment

You can declare variables separately (as we have seen) or declare them during assignment.  Use `var` only when declaring.  For example, a combined declaration and assignment would look like:

```pascal
var x: integer(8) = 5;
```

Assigned separately would look like:

```pascal
var x: integer(8);

x = 5;
```

### Conditions

Block style conditionals:

```pascal
if x < 5 begin
	io.writeln('x is less than 5');
end
else if x == 5 begin
	io.writeLn('x is equal to 5')
end
else begin
	io.writeLn('x is greater than 5');
end
```

Because the previous example only has a single statement it can be simplified:

```pascal
if x < 5 then
	io.writeLn('x is less than 5');
else if x == 5 then
	io.writeLn('x is equal to 5');
else then
	io.writeLn('x is greater than 5');
```

#### And

```pascal
if x < 5 and x <> 3 then
	io.writLn('x is less than 5, but not 3');
```

#### Or

```pascal
if x < 5 or x > 5 then
	io.writeLn('x is not 5');
```

#### Ternary

```pascal
var msg: string = case x < 5
	then 'x is less than 5'
	else 'x is greater than or equal to 5';

io.writLn(msg);
```

#### Not

```pascal
if not x < 5 then
	io.writeln('x is greater than or equal to 5')
```

### Loops

A basic for loop:

```pascal
for var x = 1 to 5 begin
	io.writeLn('x is equal to ', x);
end
```

Simplified to a single line:

```pascal
for var x = 1 to 5 do
	io.writeLn('x is equal to ', x);
```

Iterating over characters in a string:

```pascal
for var x in 'string' do
	io.writeLn('x is equal to ', x);
```

Iterating over values in an array:

```pascal
for var x in [1, 2, 3, 4, 5] do
	io.writeLn('x is equal to ', x);
```

A basic while loop:

```pascal
var x = 1;

while x < 5 begin
	io.writeLn('x is equal to ', x);
	x++;
end
```

A single line while loop;

```pascal
var x = 'string';

while x.length do
	io.writeLn(x.pop(2)); // outputs: 'ng', 'ri', 'st'
```

A repeat loop:

```pascal
var x = 1;

repeat
	io.writeLn('x is equal to ', x);
	x++;
until
	x == 5;
```

### Units

Units are effectively the namespaces of your code.  All code must belong to a unit, you declare the unit for the code as follows:

```pascal
unit app;
```

You can have a sub-units by separating the members of the unit path with a `\` character:

```pascal
unit app\utils;
```

### Uses

To "import" or use code in another unit from the currently declared unit/sub-unit, you would use the `uses` keyword:

```pascal
uses io;
```

> NOTE: The `uses` keyword operates as an alias.  Modules registered inside a unit is not actually "imported" or linked until it is used.

To use a registered module, you can separate the unit path from the module identifier with a `.` character:

```pascal
uses io.writeLn;
```

You can alias the units and or modules you're using with the `as` keyword.

```pascal
uses io.writeLn as printLn;
```

If you want to import multiple individual modules from the same unit path, you can use `()` syntax such as:

```pascal
uses io(writeLn, readLn);
```

You can combine all of these into a single `uses` statement:

```pascal
uses
	io(
		readLn,
		writeLn as printLn
	),
	crono.time,
	crypt\algos.rot13 as encrypt
;
```

### Register Modules

Once a unit is declared, you can begin registering modules (functions, types, etc) within that unit.  The shorted possible PINT program would look like the following:

```pascal
unit main;

register main: function() = 0;
```

### Functions

Functions generally look more like this:

```pascal
register main: function(argc: integer; argv: string[]): integer(8)
begin
	return 0;
end
```

The use of `return` above is actually a short hand for assigning the return value and immediately returning from the function.  We can also assign the `return` value directly and only return at the end of the function, allowing us to perform actions after our return value has been determined.

```pascal
register main: function(argc: integer; argv: string[]): integer(8)
begin
	if not var config = getConfig('/var/app/config.jin') then
		return = 1;
	else then
		return = 0;

	cleanup();
end

register cleanup: function(): void
begin
	{
		Do cleanup work here.
	}
end
```

Alternative return types allow you to return arbitrary values from a function which do not match any of its known return types.  You do this by placing a `?` symbol before the known return type:

```pascal
register getConfig: function(path: string): ?Config
begin
	if not var fd = io.openFile(path) then
		return new Error('Could not open configuration at "%s"'.format(path));
	else begin
		var config:Config = new Config();

		// parse and set configuration information
		
		return config;
	end
end
```

To capture the returned value and assert its type, use the `on` statement.  In the example below, in the event that `error` is an `Error` we iterate and print the errors so long as there are previous errors to report.

```pascal
var config: Config;

config? error = getConfig('/var/app/config.jin');

on error is Error repeat
    io.writeLn('Error: %s in function %s'.format(error.getMessage(), error.getFunction()));
    error = error.getPrevious();
until
	not error;
```

## Object Oriented Constructs

In this next section, we'll examine the object-oriented constructs of PINT.  To elucidate this, we'll imagine things in the context of trying to build a logging library.  Let's start with basic objects.

### Objects

Basic objects are not classed.  They are basically dictionaries with variant properties.

```pascal
var context   := new object()
context.left  := x;
context.right := y;
```

You can also use a literal syntax:

```pascal
var context := (
	left  := x,
	right := y
);
```

To access an object property you can use the `.` notation when the identifier is static.  If the identifier needs to be a variable you can use `[]` notation:

```pascal
var key := 'left';

return context[key];
```

### Labels

Labels are PINT's form of enumerators.

```pascal
unit Logger;

register Level: label = (
	INFO,
	WARNING,
	ERROR
);
```

If you're extending an existing library, it's often the case that you may need to add members.  Accordingly you can extend labels:

```pascal
unit ContextLogger;

uses Logger;

register Level: label(Logger\Level) = (
	CONTEXT
);
```

Labels are accessed using a backslash notation:

```pascal
Level#INFO
```

### Types

You can define custom types which are a union of multiple types using the `type` form.

```pascal
unit ContextLogger;

register Context: type = (
	object,
	cardinal
);
```

Then use them:

```pascal
unit app;

uses ContextLogger.Context;

var context1: Context = 1;
var context2: Context = (id := id);
```

### Records

Records are basically structs.  They're like objects, but their properties are declared in advance and can have default values assigned at the time of instantiation.  In the example below, the `timestamp` would reflect the return value of `time()` at the time an `Entry` record is instantiated (not when declared).

```pascal
unit Logger;

uses crono.time;

register Entry: record = (
	text: string,
	level: LogLevel,
	timestamp: cardinal default time()
);
```

Like labels, records can extend other records:

```pascal
unit ContextLogger;

uses Logger;

register Entry: record(Logger\Entry) = (
	context: Context
);
```

To use a record, you need to use the `new` keyword to create an instance:

```pascal
uses ContextLogger(Entry, Level);

var entry := new Entry(
	text    = 'Cannot find record of type %s'.format(User),
	level   = Level\WARNING,
	context = (
		id := id
	)
);
```

Any property which does not have a default initialization is required to be set upon instantiation.

### Interfaces

Interfaces are an easy way that you can define methods which may be common across multiple implementations.  All defined interface methods are public.  Continuing with our logger example, we might define a simple logger interface as follows:

```pascal
unit Logger;

register Log: interface
begin
	const log: function(entry: Entry): ?self;
end
```

Extending that as well:

```pascal
unit ContextLogger;

uses Logger;

register Log: interface(Logger\Log)
begin
	const log: function(entry: Entry): ?self;
end
```

### Implementations

Implementations are like traits, they represent horizontally sharable functionality, they can offer implementations of interfaces:

```pascal
unit Core;

uses io, crono, json, ContextLogger(Log, Entry);

register FileLogger(Log): implementation
begin
	const log: function(entry: Entry): ?Log
	begin
		var today := new crono.Date('today');
		var fd    := io.openFile(
			'storage/logs/%s.log'.format(today.format('Y-m-d')),
			io.Mode\APPEND
		);
		
		if not fd then
			return = new Error('Could not open log file');
		else begin
			io.writeLine('%s: %s %s'.format(
				entry.level,
				entry.txt,
				json.encode(entry.context)
			);

			io.close(handle);
			
			return = this;
		end
		
		
	end
end
```

### Classes

Unlike other object-oriented types, classes cannot extend other classes.  They can, however, implement interfaces (like implementations can), or use implementations.  In the example below we use our previously defined `FileLogger` implementation, that implements `ContextLogger.Log` to extend it's functionality to our `Application`:

```pascal
unit Core;

use crono, ContextLogger(Entry, Level);

register Application: class(FileLogger)
begin
	public:
		const boot: function(): self
		begin
			var start = crono.time();
			
			// Do booting stuff
			
			this.log(new Entry(
				text    = 'Finished booting',
				level   = Level\CONTEXT,
				context = (
					time := crono.time() - start
				)
			));
		end
end
```

### Generics

These are just some ideas for generics

```pascal
unit DI;

register Injector: class()
begin
	public:
		const get: function(class: class<T>): T
		begin
			return new T();
		end
end
```

```pascal
unit DB;

register EntityId: type(): (
	integer,
	string,
	object
);

register Repository: implementation<T of Entity>()
begin
	protected:
		const manager: Manager;

	public:
		const constructor(manager: Manager): void
		begin
			this.manager = manager;
		end
		
		const find: function(id: EntityId): ?T
		begin
			{find the entity and return it or return nil if not found}
		end
end

register Manager: class()
begin
	private:
		const repositories: object;

	public:
		const constructor()
		begin
			this.repositories = new object();
		end
	
		const getRepository: function(class: class<T of Repository>): T
		begin
			if not set this.repositories[class] begin
				this.repositories[class] := new T(this);
			end

			return this.repositories[class];
		end
end
```

```pascal
unit App;

register Person: class()
begin
	protected:
		var id: integer;
end

register People: class(Repository<Person>)
begin

end
```

```pascal
unit main;

uses DI, DB, App;

register main: function(): integer
begin
	var manager := DI.Injector.get(DB.Manager);
	var people  := manager.getRepository(App.People);
	var person  := people.find((email: 'info@dotink.org'));
end
```

