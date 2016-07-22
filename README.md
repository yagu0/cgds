# cgds: C Generic Data Structures library</h1>

Various data structures, from stack to tree, which can contain any data type.</p>

## Example

	Vector v = vector_new(int);
	vector_push(v, 32);
	vector_push(v, 42);
	int a; vector_get(v, 1, a); //a now contains 42
	vector_set(v, 0, 0); //v[0] now contains 0
	vector_destroy(v);</code></pre>

More examples in the unit tests under test/ folder.</p>
