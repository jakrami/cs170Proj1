First I have a main function that checks for the -n flag, and I pass that bool
into the actual shell function, for there I have a while loop that get cin
and keeps looping till an end of file (ctrl d); For my parsing, I use a string
stream to get each word, then I pass it into a function that separates the input
into multiple words in case there are meta characters that are stuck to words. I
store this in a vector.

Then I go through that vector of each individual word and keep extracting until
I reach a |, then I take all of the input before the pipe and put it into another
vector of vectors to keep track. When I do my executions, I have two separate cases
for whether it is one command or multiple with piping. To help me with catching
zombies, I use signal(SigCHLD, handler), which just waits for the child process
to be picked up while allowing functionality of the parent.

I think the most trouble I had with this project is the parsing of it, there
were a lot of little edge cases that I kept missing and I'm sure I still missed
some of then. I think a lot of this happens in my split word function. 
