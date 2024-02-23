# Pascal Inspired New-Talk (PINT)

PINT (like beer) is a **hypothetical** object-oriented programming language _without_ class inheritance.  Its syntax is heavily inspired by Pascal, so much so that you can obtain syntax highlighting generally by just enabling any `.pint` file to use a Pascal language file.  Unlike Pascal, PINT has a number of more modern features which aim to reduce repetition and enhance availability of run-time information out of the box.

PINT is neither compiled, nor interpreted, because it's not real **yet**.  However, the ultimate aim is to have a compiled language with flexibility in choice of dynamic (less efficient) or type-safe (more efficient) features and data structures.

## Everything Is An Object

Integers?  Yep, that's an object.  Functions? Objects.  Interfaces?  Yes, stop asking.  Technically, they're all structs, but a struct is just an object without function pointers.  To further elucidate this point, let's look at a basic variable declaration.

```pascal
var x: integer(8);
```

While this looks like a simple memory allocation (get me 8-bits and reference the pointer as x), the above code is actually creating an object behind the scenes.  These objects may differ slightly in their properties depending on the core "type" being created, but for an integer and most scalar types you can imagine it as follows:

```json
{
    type: "integer",  // The the type of the object
    size: 8,          // The representative size of the object (type specific)
    pntr: null,       // Where data for this object is stored in memory
    impl: <bm>        // An 8-bit bitmask flagging type-safety, size-safety, etc.
}
```

Basically, with the exception of labels, anywhere you see a `:` whatever follows is an object type declaration.  Additional examples include, defining interfaces, classes, and functions:

```pascal
const main: function(argc: integer, argv: string[]): integer
begin
	return 0;
break;
```

In the above example, we're creating four objects.  See if you can spot them all!

## Variables, Types and Sizes

PINT tries to reach a middle ground between dynamic and statically typed languages in that compatible values can be dynamically typed or resized depending on how they were declared.  To declare a variable you use the `var` keyword.

Declaration and assignment can occur in a single line:

```pascal
var x: integer(32) = 5;
```

Or in multiple separately:

```pascal
var x: integer(32);

x = 5;
```

Although the default `integer` size is 32-bits, _how_ a variable is declared carries additional meaning.  In the following example, when the assignment takes place, the const of `5` (a 32-bit integer) is checked to see if it will fit into the smaller `x` (a 16-bit integer) due to its size being larger.  Since it is "in bounds," the assignment can proceed, despite that `x` is declared explicitly as a 16-bit integer:

```pascal
var x: integer(16) = 5;  // sizeOf(x) == 16
```

By contrast, the example below would result in an error because the integer `1024` does not fit within the explicitly declared `size` of 8-bits.

```pascal
var x: integer(8) = 1024;  // Error
```

> Error: cannot assign integer(32) to integer(8), out of bounds.

Declaring variables in this manner results in them being both type-safe and size-safe, which are two distinct concepts in PINT.

It also possible, however, for a variable to be declared with a dynamic size.  In the example below we declare `x` as an integer, but we do not specify a size during the declaration by leaving off the `(#)` notation.

```pascal
var x: integer;
```

Although this `integer` will still default to 32 bits, this means that its size will be automatically adjusted to match an assigned value.  This can be demonstrated by explicitly casting the `integer` we assign to it as something smaller and then checking it:

```pascal
var x: integer = 128:integer(8);  // sizeOf(x) == 8
```

Similarly it can be reallocated to accommodate larger sizes after the fact.  The following re-assignment would produce no error, and `x` would be resized as needed:

```pascal
x = 1024;  // sizeOf(x) == 32
```

### Implicit Declarations Through Assignment

It is possible to implicitly declare the type of a variable through assignment by leaving off the type.  Variables declared in this way will retain their type-safety, but will not have size-safety:

```pascal
var x: = 5;
```

To retain type safety with an implicit declaration you would simply do:

```pascal
var x:() = 5;
```

Similarly, you can declare an explicit size and get an implicit type:

```pascal
var x:(8) = 5;
```

Lastly, it is possible to declare a variable in such a way that is has neither type-safety, nor size-safety:

```pascal
var x = 5;
```

In this case the the variable `x` could be assigned rather arbitrarily:

```pascal
x = 1024;   // typeOf(x) == 'integer'
x = 10.2;   // typeOf(x) == 'real'
x = 'str';  // typeOf(x) == 'string'
x = 'c';    // typeOf(x) == 'char'
```

### Chars and Strings

Characters and strings in PINT are in Unicode Transformation Format (UTF).  By default, they are UTF-8.

```pascal
var c = 'c';  // sizeOf(c) == 8, typeOf(c) == 'char'
```

Similar to integers, cardinals (unsigned integers), and real numbers (floats), you can define their size when you declare them:

```pascal
var c: char(16) = 'c';  // sizeOf(c) == 16
```

> IMPORTANT: Up until this point we have only dealt with scalar values which, by and large, fit into the declared sizes.  The `size` property of value should not be understood to be the actual amount of memory consumed in its storage, nor is it strictly speaking a count of the elements contained therein.  Rather it is a representational size in terms which are different but meaningful for each type.
>
> - For scalars like numbers, booleans, and bitmasks, their objects represent bits.  Accordingly the size is the number of bits they represent.
> - For characters and strings, their objects represent UTF characters.  Accordingly their size is a UTF unit bit-size.
> - For complex types like arrays, objects, interfaces... their size will generally represent the number of elements, properties, functions, etc.
>
> The `sizeOf` function always reports this representational size.  To determine the size of the allocated memory you would use `bytesOf()`.  To determine the length of value in terms of what it represents you would use `lengthOf()`.

Strings operate on the same principle, but unlike some other languages the type of quotes don't suggest anything about the type.  Consider these implicit declarations:

```pascal
var c: = 'c'    // typeOf(c) == 'char'
var s: = 'str'  // typeOf(s) == 'string'
```

Assigning a character to an explicitly typed `string` is always fine:

```pascal
var s: string(8) = 'c';
```

Assigning a string to an explicitly type `char` is fine if its only a single character:

```pascal
var c: char = 's':string;
```

But, similar to trying to assign an `integer(32)` to an `integer(16)` the value will first be checked to determine if it's in bounds:

```pascal
var c: char = 'string';
```

> Error: cannot assign string(8) to char(8), out of bounds.

Finally, strings act like arrays in that you can obtain each character within the string via and index:

```pascal
var c: char = 'string'[0];  // c == 's'
```

### Arrays

Arrays basically look as you would anticipate based on most other languages and are declare similar to other variables.  You can have arrays of any type of values and those values can be type-safe and size-safe depending on how you've declared them.  As you might expect, you can also declare the `size` of an array itself:

```pascal
var numbers: integer(8)[10];
```

Attempting to add onto an array (which is already full) would result in an error:

```pascal
numbers[] = 10;
```

> Error: cannot assign integer(32) to integer(8)[], too many elements.

Similarly, you'd receive an error trying to assign an incompatible value even if the array still had room:

```pascal
numbers[] = 1024;
```

> Error: cannot assign integer(32) to integer(8)[], out of bounds.

Let's look at a few more example declarations starting with an array whose values are type and size-safe, but whose own `size` is dynamic:

```pascal
var numbers: integer(8)[];
```

We can remove the size-safety of its values:

```pascal
var numbers: integer[];
```

We can remove the type-safety altogether:

```pascal
var things[];
```

There are array literals, so you can assign an array to an array:

```pascal
var things[] = [1, 2.5, 'c', 'string'];
```

And there are operations:

```pascal
things = things + [1, 5, 'b'];  // Adds elements from right side not matched in left side
things = things - ['c', 7.5];   // Removes elements from left side matched in right side
things = things ~ ['cats', 2];  // Concatenate elements from right side to end of left side
```



```pascal
var thing: 

thing.name = 'string';
thing.age  = 18;
```

