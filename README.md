### Introduction

Implementation of a virtual data storage based on UFS.

<h1>Run</h1>

```
make
```
```
./usf
```
<h2>Option</h2>

```
Eng:
------------------ Menu --------------------------------
1 - Execute a command (format, ls, mkdir, create, rm).
2 - run command in a file 
0 - exit.
--------------------------------------------------------

fr:
------------------ Menu --------------------------------
1 - Executer une commande (format, ls, mkdir, create, rm).
2 - Executer plusieurs commandes se trouvant dans un fichier texte.
0 - Quitter.
--------------------------------------------------------

Entrer s.v.p. votre choix (0 a 2):? 1
Entrez le nom du fichier: (format, ls, mkdir, create, rm) /home

Entrer s.v.p. votre choix (0 a 2):? 2
Entrez le nom du fichier: "Filename"
```
<h1>Tests</h1>
<h2>Test1</h2>
output:

``` 
===== Commande format =====
UFS: saisir i-node 1
UFS: saisir bloc   25

===== Commande ls / =====
/
d .             Size: 56                iNode: 1                nlink: 2
d ..            Size: 56                iNode: 1                nlink: 2

===== Commande mkdir /doc =====
UFS: saisir i-node 2
UFS: saisir bloc   26

===== Commande ls / =====
/
d .             Size: 112               iNode: 1                nlink: 3
d ..            Size: 112               iNode: 1                nlink: 3
d doc           Size: 56                iNode: 2                nlink: 2

===== Commande mkdir /tmp =====
UFS: saisir i-node 3
UFS: saisir bloc   27

===== Commande ls / =====
/
d .             Size: 168               iNode: 1                nlink: 4
d ..            Size: 168               iNode: 1                nlink: 4
d doc           Size: 56                iNode: 2                nlink: 2
d tmp           Size: 56                iNode: 3                nlink: 2

===== Commande mkdir /tmp/lib =====
UFS: saisir i-node 4
UFS: saisir bloc   28

===== Commande ls / =====
/
d .             Size: 224               iNode: 1                nlink: 4
d ..            Size: 224               iNode: 1                nlink: 4
d doc           Size: 56                iNode: 2                nlink: 2
d tmp           Size: 84                iNode: 3                nlink: 3

===== Commande ls /tmp =====
/tmp
d .             Size: 84                iNode: 3                nlink: 3
d ..            Size: 224               iNode: 1                nlink: 4
d lib           Size: 56                iNode: 4                nlink: 2

===== Commande mkdir /tmp/lib/deep =====
UFS: saisir i-node 5
UFS: saisir bloc   29

===== Commande ls /tmp/lib/deep =====
/tmp/lib/deep
d .             Size: 56                iNode: 5                nlink: 2
d ..            Size: 84                iNode: 4                nlink: 3

===== Commande rm /tmp/lib/deep =====
UFS: Relache i-node 5
UFS: Relache bloc   29

===== Commande rm /tmp/lib =====
UFS: Relache i-node 4
UFS: Relache bloc   28

===== Commande rm /tmp =====
UFS: Relache i-node 3
UFS: Relache bloc   27

===== Commande ls / =====
/
d .             Size: 140               iNode: 1                nlink: 3
d ..            Size: 140               iNode: 1                nlink: 3
d doc           Size: 56                iNode: 2                nlink: 2

===== Commande create /a.txt =====
UFS: saisir i-node 3

===== Commande ls / =====
/
d .             Size: 140               iNode: 1                nlink: 4
d ..            Size: 140               iNode: 1                nlink: 4
d doc           Size: 56                iNode: 2                nlink: 2
- a.txt         Size: 0         iNode: 3                nlink: 1

===== Commande rm /a.txt =====
UFS: Relache i-node 3

===== Commande ls / =====
/
d .             Size: 112               iNode: 1                nlink: 3
d ..            Size: 112               iNode: 1                nlink: 3
d doc           Size: 56                iNode: 2                nlink: 2

===== Commande format =====
UFS: saisir i-node 1
UFS: saisir bloc   25

===== Commande ls / =====
/
d .             Size: 56                iNode: 1                nlink: 2
d ..            Size: 56                iNode: 1                nlink: 2

```
<h2>Test2</h2>
output:

``` 
===== Commande format =====
UFS: saisir i-node 1
UFS: saisir bloc   25

===== Commande ls / =====
/
d .             Size: 56                iNode: 1                nlink: 2
d ..            Size: 56                iNode: 1                nlink: 2

===== Commande ls /doc =====
Directory doesn't exist
===== Commande mkdir /new =====
UFS: saisir i-node 2
UFS: saisir bloc   26

===== Commande mkdir /empty =====
UFS: saisir i-node 3
UFS: saisir bloc   27

===== Commande mkdir /notempty =====
UFS: saisir i-node 4
UFS: saisir bloc   28

===== Commande mkdir /existepas/new =====
UFS: saisir i-node 5
UFS: saisir bloc   29
UFS: saisir i-node 6
UFS: saisir bloc   30

===== Commande create /notempty/c.txt =====
Directory already exist!
UFS: saisir i-node 7

===== Commande create /b.txt =====
UFS: saisir i-node 8

===== Commande create /existepas/b.txt =====
Directory already exist!
UFS: saisir i-node 9

===== Commande ls / =====
/
d .             Size: 336               iNode: 1                nlink: 7
d ..            Size: 336               iNode: 1                nlink: 7
d new           Size: 56                iNode: 2                nlink: 2
d empty         Size: 56                iNode: 3                nlink: 2
d notempty              Size: 56                iNode: 4                nlink: 3
d existepas             Size: 84                iNode: 5                nlink: 4
- b.txt         Size: 0         iNode: 8                nlink: 1

===== Commande ls /notempty =====
/notempty
d .             Size: 56                iNode: 4                nlink: 3
d ..            Size: 336               iNode: 1                nlink: 7
- c.txt         Size: 0         iNode: 7                nlink: 1

===== Commande rm /notempty =====

===== Commande ls / =====
/
d .             Size: 336               iNode: 1                nlink: 7
d ..            Size: 336               iNode: 1                nlink: 7
d new           Size: 56                iNode: 2                nlink: 2
d empty         Size: 56                iNode: 3                nlink: 2
d notempty              Size: 56                iNode: 4                nlink: 2
d existepas             Size: 84                iNode: 5                nlink: 4
- b.txt         Size: 0         iNode: 8                nlink: 1

===== Commande rm /notempty/c.txt =====
UFS: Relache i-node 7

===== Commande rm /notempty =====
UFS: Relache i-node 4
UFS: Relache bloc   28

===== Commande ls / =====
/
d .             Size: 252               iNode: 1                nlink: 6
d ..            Size: 252               iNode: 1                nlink: 6
d new           Size: 56                iNode: 2                nlink: 2
d empty         Size: 56                iNode: 3                nlink: 2
d existepas             Size: 84                iNode: 5                nlink: 4
- b.txt         Size: 0         iNode: 8                nlink: 1

===== Commande rm /a.txt =====

===== Commande ls / =====
/
d .             Size: 252               iNode: 1                nlink: 6
d ..            Size: 252               iNode: 1                nlink: 6
d new           Size: 56                iNode: 2                nlink: 2
d empty         Size: 56                iNode: 3                nlink: 2
d existepas             Size: 84                iNode: 5                nlink: 4
- b.txt         Size: 0         iNode: 8                nlink: 1

===== Commande format =====
UFS: saisir i-node 1
UFS: saisir bloc   25

===== Commande ls / =====
/
d .             Size: 56                iNode: 1                nlink: 2
d ..            Size: 56                iNode: 1                nlink: 2
```
