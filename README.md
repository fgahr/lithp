# Lithp: the Lisp with a lisp
A very simple and (as of writing) incomplete Lisp implementation.
In no way do I want to make fun of or otherwise belittle those who speak with a
lisp. I simply couldn't resist the pun.

# How do I use it?
Since it's unfinished, you're probably better off just not using it.
There's a lot of polished, community-driven Lisp implementations out there, all
of which are strictly better than Lithp.

If, however, you insist:
```
# Build it (requires make and clang++)
# Add `-j 8` (or more) to speed up the build process
$ make
# Run it (example interaction)
$ bin/lithp
> (define (map f seq)
     (if (null? seq)
         nil
         (cons (f (car seq))
               (map f (cdr seq)))))
<lambda#0x7f5b82410250>
> (map (lambda (x) (* 2 x)) '(1 2 3))
(2 4 6)
>
```
Press Ctrl-C or Ctrl-D to exit.

The above shows a small sample interaction. The part with the ((many)
((parentheses))) is Lithp code that is, at this point, mostly a (tiny) subset of
[Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)), although
it differs in some details. If you have a large session, it will crash because
garbage collection is currently broken (see [below](#missing))

# What is it?
A personal learning project, cf. [below](#learned).
At this point, it's essentially an incomplete Scheme implementation. I intend to
add what's (urgently) missing, polish it a bit, fiddle around with it a little,
then call it a day. 

It is based on the description of Lisp implementation in the classic MIT book
[Structure and Interpretation of Computer
Programs](https://mitpress.mit.edu/sites/default/files/sicp/index.html).
Since the example implementation therein is written in Scheme and glosses over
some of the more gory details (e.g. garbage collection), translating it to a
host language without a garbage-collected runtime proved to be a nice challenge.

# What is it not?
A serious attempt at creating a full-fledged Scheme implementation or an
altogether novel language. It is a hobby project to be continued or aborted at
my leisure.

<a name="missing"/>

# What's missing?
In order of priority
* garbage collection: partially implemented but currently broken
* closures: although somewhat supported, closing over variables and keeping them
  alive is very brittle and can break at any time during evaluation
* file I/O: not implemented yet, probably not hard
* special forms should be lithp::Objects in the global environment to make
  certain operations more consistent and allow `type-of` information for them
* standard library: currently everything is built-in, but most of the standard
  library should eventually be written in Lithp
* complete heap: some object types will store data outside the managed heap.
  This does not mean that the memory is forgotten by the garbage collector, but
  seems wrong. In particular, strings are currently C++ std::string objects with
  storage in the C++ heap, evaluation environments are kept outside the heap
  altogether (this interplays with the earlier point about closures)
* numbers other than integers: currently only 64-bit integers are supported.
  Having arbitrary-sized integers, fractions, floats, and complex numbers of
  various kinds seems desirable but not critical. I might do it out of interest
* better REPL: completion is way out of scope at this moment but some niceties
  such as a command history might happen
* macros: not actually a priority, I might skip this one for a while
* lots of other things that didn't even make this list

# Why did I do it?
I needed a non-trivial project I could reasonably put together in C or C++ since
I consider myself fairly proficient in these languages (not expert-level) but
without experience with regards to project organization and refactoring and such
things that are only learned through projects.

Writing a Lisp had been on my list for a while and I had a few half-hearted and
therefore failed attempts. Recovering from a surgery I decided to try again.
Because I wanted to get a deep understanding and appreciation for all design
decisions, I skipped all of the numerous detailed materials on the matter and
relied solely on the sketches in the SICP book.

I deemed doing this in a memory-managed language such as Golang or Java
unsatisfactory because I was particularly interested in memory management so C++
seemed perfect for the job. As it turns out, I ended up with very C-ish C++ code
because I couldn't get much use out of the smart pointer types and in case of
both the stack and the heap a simple C-style array was superior to both
std::vector and std::array. I could get a bit more use out of templates but
there wouldn't be a huge advantage over some of the macros I'm using.
Summarizing, I could probably write the whole thing in C and not end up missing
much. 

<a name="learned"/>

# What did I learn?
Besides learning some C++, I gained a deeper understanding of language
implementation, in particular regarding the hand-off point between the
host language and the implemented language. I also got a deeper appreciation for
Lisp's homoiconicity (the language is described as and indistinguishable from
data) because it obviates the need for an intermediate representation between
parsing and evaluation, making the implementation rather compact.

I also learned to throw away large portions of code as I was searching for
better solutions. Although this skill can be applied (but not learned) in
excess, the dense, interwoven structure of a programming language made it more
necessary than usual.
