# Family Tree Manager.

This programm manages a familty tree (see diagrem), whose main focuse is on **recursive programming** and **memory allocation and freeing, structures** and **pointers**.

The programm supports the following features:

1. Add a family head.
    * We say the a person is a familiy head if he doesn't have a known parents in the tree.
    each person must have a unique name; the programm supports a name with unlimited characters using memory allocation technique.

2. Merry two people.

    * Two people can be merried if they don't have a common ancestor and are at the appropiate age (18) and also non of then is already married.

3. Add a new descendant.

    * Two merried people can register a new baby. a new baby get's the age of 0.

4. Print a family tree.

    * Given a family head the programm will print it's family tree starting with him, where 
    the descendants being printed in the order in which they where added to the tree which is also by their age.
    In every generation descending a tab will be printed and each couple will be printed at the same line.

Example:

/relationTree.png

```
King Charles (74) - Princess Diana (61)
    Prince William (41) - Catherine Middleton (42)
        Prince George (10)
        Princess Charlotte (8)
        Prince Louis (5)
    Prince Harry (39) - Meghan Markle (41)
        Prince Archie (4)
        Princess Lilibeth (2) 
```

5. Add given amount of years to any person in the tree.

6. Counting the number of people in the tree.

7. Print kin from a given degree sorted by name alphabetically.

    * Given a person name and the degree of closeness the all the reatives of that degree will be printed in the same way. 

To be precise:
Deg 0: the person itself.  
Deg 1: the person itself and its brothers and sisters.  
Deg 2: the person itself and its cousins who shares the same grandfather and grandmother.  
Deg 3: the person itself and all of those who shares a great grandfather with him.  
And so on.  

-

To test the code with the relation tree such as in the diagram include the input.txt in the root directory and simply run:

```
gcc familyTree.c
```
and then run:
```
./a.out < input.txt
```
You can also project the output into a result.txt file by executing:
```
./a.out < input.txt > res.txt
```

In this test we printing the family tree of Yaniv and Yuval and the relative of 2'nd degree of Eran on of 3'rd degree of Tamar:

```
Yaniv (67) - Yarden (64)
	Shalom (44)
	Dan (44) - Sharon (74)
		Eitan (22)
		Eran (22) - Michal (74)
			Asaf (0)
		David (22)
	Yeal (44)
	Daniel (44)
	Stav (44) - Alon (44)
		Shani (22)
		Sivan (22) - Yarin (74)
			Yaara (0)
			Tamar (0)

Yuval (69) - Elad (64)
	Alon (44) - Stav (44)
		Shani (22)
		Sivan (22) - Yarin (74)
			Yaara (0)
			Tamar (0)
	Einav (44)

Cousins:
David
Eitan
Eran
Shani
Sivan


Cousins:
Asaf
Tamar
Yaara

```

You can also execute you'r oun test by changing the input.txt file, for example to print Dan sub tree simply appent to the input file:
```
4
Dan
```

and in that way you can appennd any command that you want.


