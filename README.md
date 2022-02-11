<!DOCTYPE html>
<html lang="en">
  <head>
    <pre>
 _____ ______       ________      _____ ______       ________     
|\   _ \  _   \    |\   __  \    |\   _ \  _   \    |\   __  \    
\ \  \\\__\ \  \   \ \  \|\  \   \ \  \\\__\ \  \   \ \  \|\  \   
 \ \  \\|__| \  \   \ \   __  \   \ \  \\|__| \  \   \ \   __  \  
  \ \  \    \ \  \   \ \  \ \  \   \ \  \    \ \  \   \ \  \ \  \ 
   \ \__\    \ \__\   \ \__\ \__\   \ \__\    \ \__\   \ \__\ \__\
    \|__|     \|__|    \|__|\|__|    \|__|     \|__|    \|__|\|__|
    </pre>
    <p>M.A.M.A is the CS 450 project of Mohammad Alenezi, Abdullah Alqallaf, Maximillian Campbell, and Austin Williams. MAMA was developed from August 2021 to December 2021.</p>
    <h1>Table of Contents</h1>
    <ul>
      <li>Introduction</li>
      <li>Contributions</li>
      <li>Features</li>
      <li>Usage</li>
      <ul>
        <li>Booting MAMA</li>
        <li>Commands</li>
      </ul>
    </ul>
  </head>
  
  <body>
    <h1>Introduction</h1>
    <p>MAMA was the operating system (OS) that our team had developed for the semester long project for CS 450 - Operating Systems Structures. CS 450 acted as an encapsulation of all the programming techniques and tools that have been taught at West Virginia University (WVU). The MAMA team used a lot of blood, sweat, and tears (especially tears) to implement MAMA and create an OS that we are proud of. We hope you enjoy it!</p>
    <h1>Contributions</h1>
    <p>For a list of contributions, check out our <a href="WhoDidWhat.md">WhoDidWhat</a>. This table breaks down which member helped implement, test, and write a method/function of MAMA. Special thanks to Dr. Hayhurst for offering all her knowledge and <b>HUGE</b> special thanks to the TA Samuel Price for helping with debugging.</p>
    <h1>Features</h1>
    <p>MAMA is a commandline-interface (CLI) OS and implements many basic features of an OS. Some important features include:</p>
    <ul>
      <li>Interrupt driven IO</li>
      <li>Multi-process/Multi-programming</li>
      <li>Dynamic memory manager</li>
      <li>Context switching of process control blocks (PCB)</li>
      <li>Time & date function</li>
      <li>Alarms</li>
      <li>And much, much more...</li>
    </ul>
    <h1>Usage</h1>
    <h2>Booting MAMA</h2>
    <p>MAMA does have a few dependencies</p>
    <ul>
      <li>make</li>
      <li>nasm</li>
      <li>gcc</li>
      <li>qemu-system-i386</li>
    </ul>
    <p>However, you are probably better off downloading <a href="https://lcseesystems.wvu.edu/services/loud">WVU LOUD</a>, a virtual machine (VM) which already houses a lot of this software. To boot MAMA, a shell script has been provided which cleans, makes, and boots MAMA for you. Simply type: <b>./run</b> in your terminal.</p>
    <h2>Commands</h2>
    <p><a href="docs/command_cheat_sheet.md">Here</a> is a list of all the commands that MAMA is able to handle. The command cheat sheet provides brief syntax reference for each command implemented in the system as well as an example.</p>
    <p>For more information on each command, refer to the <a href="docs/MAMA User Manual.pdf">user manual</a>. The user manual provides more information on the available commands.</p>
    <p>For a more in-depth look at the code that was written refer to the <a href="docs/MAMA.pdf">programmers manual</a>. This is where to find more information on individual functions, structs, constants and other code documentation.</p>
  </body>
</html>
