# Text_Formatting_Program
A basic text editor which allows a user to describe the layout of documents using certain commands. Written in C by Wen Yee ANG as part of an assignment for COMP10002 Foundations of Algorithms at the University of Melbourne in Semester 2, 2019.

Overall mark of 13/15.

The program functions in the following ways:
- read text input from stdin.
- replace all instances of (multiple) whitespace characters (blanks, tabs, newlines) by single blanks.
- re-insert newline characters so that each output line emitted is as long as possible, but not longer than 50 characters (with the exception that if there is an input token/word that is greater than 50 characters).
- processes the following commands (which are written on lines which commence with a period character):

  - .b : break the current line.
  - .p : leave a blank line and start a new paragraph.
  - .l nn : alter the left margin from its current value (default initial value of 4) to the new value nn and start a new paragraph.
  - .w nn : alter the line width from its current value (default initial value of 50) to the new value nn and start a new paragraph. 
  - .c : take the remaining contents of this line and center them within the current output width.
  - .h nn : take the remainder of the contents of this line and use them as a section heading at the level indicated by nn.
