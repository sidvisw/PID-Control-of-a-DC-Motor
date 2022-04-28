# PID Control of a DC Motor
A basic **PID- Proportional, Integrative, Differentiative** Controller for a **DC Motor with an Encoder**.</br>

The salient features of PID Control are:
<ul>
  <li><b>Proprtional</b> control linearly adjusts the error in order to minimise it.</li>
  <li><b>Integral</b> control accumulated the past error to give a jerk free control.</li>
  <li><b>Differential</b> control takes in consideration the future aspect to make the control steady.</li>
</ul></br>

This repository contains an Arduino code for position as well as velocity control of the DC Motor.</br>
The two modes can be switched by the use of a push button.</br>
Steps to simulate the simulation on Proteus:
<ol>
  <li>Compile the Arduino Code in an Arduino IDE by turning on the compiling options.</li>
  <li>Now copy the <b>.hex</b> file path that is appearing in the IDE.</li>
  <li>Open the <b>.pdsprj</b> file in Proteus Simulator.</li>
  <li>Double click on the Arduino in the circuit file opened and paste the <b>.hex</b> file path.</li>
  <li>NOw run the simultion to see the <b>PID Control</b> working.</li>
  <li>You can control the position and speed of the <b>DC Motor</b> by using the potentiometer and the push button.</li>
</ol>
