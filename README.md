# 8 Queens Problem
<p>This is a solution for placing 8 queens on a chess board - position queens so that no queen can take another.</p>
<p>I wrote this for fun... I've been trying to find a solution to this problem that I wrote during grad
   school a looong time ago.  That solution was in lisp.  I hope to find it somewhere as I go through
   old disks/notebooks but figured I'd take another stab at the problem in c/c++ (if I find the lisp version I'll add it here).
   <br/>I know this doesn't pass 'best practices' coding, but my goal was to come up with a solution/algorithm (and this was
        just written for me so it satisfied my need :)).</p>

<p>Some notes</p>
<ul>
  <li>The program will show all the possible combinations (92 of them on 8x8 board).</li>
  <li>You can use different board dimensions, just change the DIMENSION constant (board is DIMENSION x DIMENSIOIN).  
      It's currently define as 8 (a typically chess board).</li>
  <li>You can turn on debugging (via const) or have it turned on based on a particular board position.  The code currently has 
      it getting turned on based on a particular board config (see the top of the 'doWork' function).  
      <br><strong>fyi:</strong> When debug is on you are shown the board position and various 'states'; you're 
      prompted after each display giving you the opportunity to turn debugging off.</li>
</ul>

<p>More detail</p>
<ul>
   <li>The board is stored internally as a one dimensional array.  Row/col is translated to position [(ROW*DIMENSION)+col].  Variable
       boardPositions[] is an array of booleans, if position is on then it has a queen placed there</li>
   <li>To speed up processing I have other arrays to identify if the board is 'good'.  For each of these arrays I store
       a number that identifies the number of queens at that location... doing this I know the board is good (as long
       as the value is &lt; 2).  The arrays are:
     <dl>
       <dt>colPositions[]</dt><dd>The number of queens at that column position</dd>
       <dt>rowPositions[]</dt><dd>Number of queens in that row.  Note you may say this isn't needed based on the way the code
           works but I left it in case I wanted to populate the board via another means (i.e. randomly).</dd>
       <dt>leftDiagnol[]</dt>
         <dd>Queens can move diagnolly left or right.  There are a total of (dimension * 2 - 1) possible diagnols (for left and
             right) on the board.  
             The 'left' diagnols go from right to left as they move up the board... think of them as the '\' direction.
             The first diagnol is at 0,0, next is at 0,1, it goes to 0,7 then 1,7... 7,7 (assuming dimension of 8). 
             The logic to calculate the diagnol number for any location is simple; it's just: row+col.<br/>
             This array keeps a count of the number of queens at each of the left diagnols</dd>
       <dt>rightDiagnol[]</dt>
          <dd>The count representing number of queens at each right diagnol, think of them as the '/' direction. 
              The right diagnol ordering is: 7,0 then 6,0 then 5,0... 0,0 then 0,1 and ending at 0,7.
              The logic to calculate the diagnol number for a location is: (DIMENSION - row - 1) + col<br/>
              (make sure you read leftDiagnol desc to understand diagnols).</dd>
    </dl>
   </li>
   <li>There's quite a bit of 'debug'/'helper' code here, the overall
       solution isn't really much code.  The bulk of the logic is in 
       function 'doWork(row,col)', it's high level logic is<pre>
         put a piece at row/col
         if board is good 
           call doWork(row+1,0)   // try putting piece at next row
         remove piece at row/col  // remove piece 
         call doWork(row,col+1)   // try piece at next col position </pre>
       There's a little more... if it's called with a row outside the board then we know we have a solution
       and if called with a col outside dimension we just return, but that's pretty much it.
    </li>
    <li>Also look at        
       <ul>
         <li>Function 'setPostion'/'clearPostion'/'setPos' they place a queen on the
             board or remove it (the setPos is a helper for the other two and does the real work)</li>
         <li>Function 'goodBoard' determines if the board is in a good state - meaning no queen can take another</li>         
       </ul>
    </li>
    <li>To compile (on mac) do:<pre>g++ 8Queens.cpp -o test.out -v</pre></li>
</ul>

