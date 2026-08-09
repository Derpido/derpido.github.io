//Dashboard pictures and variables
PImage Pencil_Settings;
PImage More;
PImage Exit;
PImage Instructions;
PImage Dashboard;
PImage Image;
PImage board_Settings;
PImage Pencil;
PImage Eraser;
PImage About;
PImage Instructions_Texts;
PImage Credits;
PImage Black;
PImage White;
PImage Red;
PImage Green;
PImage Blue;
PImage Small;
PImage Medium;
PImage Big;
PImage Pencil_T;
PImage Board_T;

String state = "pad" ;

PGraphics Board;

color bg_color = color(#FFFFFF);
color pencil = color(#000000);
int str = 10;

void setup(){
 size(1000,800);
 
 Exit = loadImage("Exit.png");
 Instructions = loadImage("Instructions.png");
 Pencil_Settings = loadImage("Pencil_Settings.png");
 More = loadImage("More_Button.png");
 Dashboard = loadImage("Dashboard.png");
 Image = loadImage("Image.png");
 board_Settings = loadImage("Board_Settings.png");
 Pencil = loadImage("Pencil.png");
 Eraser = loadImage("Eraser.png");
 About = loadImage("About.png");
 Instructions_Texts = loadImage("Instructions_Text.png");
 Credits = loadImage("Credits.png");
 Black = loadImage("Black.png");
 White = loadImage("White.png");
 Red = loadImage("Red.png");
 Green = loadImage("Green.png");
 Blue = loadImage("Blue.png");
 Small = loadImage("Small.png");
 Medium = loadImage("Medium.png");
 Big = loadImage("Big.png");
 Pencil_T = loadImage("Pencil_Text.png");
 Board_T = loadImage("Board_Text.png");
 
 Pencil.resize(50,50);
 Eraser.resize(50,50);
 
 Board = createGraphics(width,height);

 Board.beginDraw();
 
 Board.background(#FFFFFF);
 Board.image(Image,0,0);
 
 Board.endDraw();
 
}

void draw(){
  background(#FFFFFF);
   
  if(state == "pad"){
    pad();
    
  }
  
   if(state == "dashBoard"){
    pad();
    dashBoard();
   
  }
  
  if(state == "instructions"){
    pad();
    dashBoard();
    instructions();
   
   
  }
  
  if(state == "credits"){
    pad();
    dashBoard();
    credits();
    
  }
  
  if(state == "pencil"){
    pad();
    dashBoard();
    pencil();
    
  }
  
  if(state == "board"){
    pad();
    dashBoard();
    board();
    
  }
  
  if(state == "White"){
    pad();
    dashBoard();
    board();
    White();
    
  }
  
  if(state == "Black"){
    pad();
    dashBoard();
    board();
    Black();
    
  }
  
  if(state == "Red"){
    pad();
    dashBoard();
    board();
    Red();
    
  }
  
  if(state == "Green"){
    pad();
    dashBoard();
    board();
    Green();
    
  }
  
  if(state == "Blue"){
    pad();
    dashBoard();
    board();
    Blue();
    
  }
  
  if(state == "Small"){
    pad();
    dashBoard();
    board();
    Small();
    
  }
  
  if(state == "Medium"){
    pad();
    dashBoard();
    board();
    Medium();
    
  }
  
  if(state == "Big"){
    pad();
    dashBoard();
    board();
    Big();
    
  }
  
  if(state == "white"){
    pad();
    dashBoard();
    board();
    white();
    
  }
  
  if(state == "black"){
    pad();
    dashBoard();
    board();
    black();
    
  }
  
  if(state == "ReD"){
    pad();
    dashBoard();
    board();
    ReD();
    
  }
  
  if(state == "GreeN"){
    pad();
    dashBoard();
    board();
    GreeN();
    
  }
  
  if(state == "BluE"){
    pad();
    dashBoard();
    board();
    BluE();
    
  }

   more(); 
 
}

void dashBoard(){
 image(Dashboard,60,0,310,70);
 image(Instructions,70,10,50,50);
 image(board_Settings,130,10,50,50);
 image(Pencil_Settings,190,10,50,50);
 image(About,250,10,50,50);
 image(Exit,310,10,50,50);
 
  if(mousePressed && mouseX>70 && mouseX<70+50 && mouseY>10 && mouseY<10+50){
   state = "instructions";
   delay(100);  
   println("instruction");
  
 }
 
 if(mousePressed && mouseX>250 && mouseX <250+50 && mouseY>10 && mouseY<10+50){
   state = "credits";
   delay(100);
   println("credits");
   
 }
 
 if(mousePressed && mouseX>190 && mouseX<190+50 && mouseY>10 && mouseY<10+50){
   state = "pencil";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>130 && mouseX<130+50 && mouseY>10 && mouseY<10+50){
   state = "board";
   delay(100);
   
 }
 
}

void more(){
 image(More,10,10);
  
}

void pad(){ 
 //Writing
 if(mousePressed){
 
 
 //Marker  
 if(mouseButton==LEFT){
   cursor(Pencil,1,31);
   
   Board.beginDraw(); 
 
   Board.stroke(pencil);
   Board.strokeWeight(str);
   Board.line(pmouseX,pmouseY,mouseX,mouseY);
   
   Board.endDraw();
 }
 
 //Eraser
 if(mouseButton==RIGHT){
   cursor(Eraser,16,31);
   
   Board.beginDraw();
   
   Board.fill(bg_color);
   Board.noStroke();
   Board.ellipse(mouseX,mouseY,30,30);    
   
   Board.endDraw();
   
 }

 }
 
 else{
  cursor(HAND); 
   
 }
 
 image(Board,0,0);
 
}
 
void instructions(){
 image(Instructions_Texts,250,200); 
 
}

void credits(){
 image(Credits,250,200); 
  
}

void pencil(){
 image(Pencil_T,250,200);
 image(White,300,450);
 image(Black,360,450);
 image(Red,420,450);
 image(Green,480,450);
 image(Blue,540,450);
 image(Small,360,510);
 image(Medium,420,510);
 image(Big,480,510);
 
 if(mousePressed && mouseX>300 && mouseX<300+50 && mouseY>450 && mouseY<450+50){
   pencil = color(#FFFFFF);
   mark();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>360 && mouseX<360+50 && mouseY>450 && mouseY<450+50){
   pencil = color(#000000);
   mark();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>420 && mouseX<420+50 && mouseY>450 && mouseY<450+50){
   pencil = color(#F50A0A);
   mark();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>480 && mouseX<480+50 && mouseY>450 && mouseY<450+50){
   pencil = color(#2CB723);
   mark();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>540 && mouseX<540+50 && mouseY>450 && mouseY<450+50){
   pencil = color(#24B4DE);
   mark();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>360 && mouseX<360+50 && mouseY>510 && mouseY<510+50){
    str = 1;
    delay(100);
   
 }
 
  if(mousePressed && mouseX>420 && mouseX<420+50 && mouseY>510 && mouseY<510+50){
    str = 5;
    delay(100);
   
 }
 
  if(mousePressed && mouseX>480 && mouseX<480+50 && mouseY>510 && mouseY<510+50){
   str = 10;
   delay(100);
   
 }

}

void board(){
 image(Board_T,250,200); 
 image(White,300,450);
 image(Black,360,450);
 image(Red,420,450);
 image(Green,480,450);
 image(Blue,540,450);
 
  if(mousePressed && mouseX>300 && mouseX<300+50 && mouseY>450 && mouseY<450+50){
   bg_color = color(#FFFFFF);
   reset();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>360 && mouseX<360+50 && mouseY>450 && mouseY<450+50){
   bg_color = color(#000000);
   reset();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>420 && mouseX<420+50 && mouseY>450 && mouseY<450+50){
   bg_color = color(#F50A0A);
   reset();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>480 && mouseX<480+50 && mouseY>450 && mouseY<450+50){
   bg_color = color(#2CB723);
   reset();
   delay(100);
   
 }
 
 if(mousePressed && mouseX>540 && mouseX<540+50 && mouseY>450 && mouseY<450+50){
   bg_color = color(#24B4DE); 
   reset();
   delay(100);
   
 }
   
}

void White(){
  image(White,300,450);
  
}

void Black(){
  image(White,360,450);
  
}

void Red(){
  image(White,420,450);
  
}

void Green(){
  image(White,480,450);
  
}

void Blue(){
  image(White,540,450);
  
}

void Small(){
  image(Small,360,510);
  
}

void Medium(){
  image(Medium,420,510);
  
}

void Big(){
  image(Big,480,510);
  
}

void white(){
  image(White,300,450);
  
}

void black(){
  image(Black,360,450);
  
}

void ReD(){
  image(Red,420,450);
  
}

void GreeN(){
  image(Green,480,450);
  
}

void BluE(){
  image(Blue,540,450);
  
} 

void reset(){
  Board.beginDraw();
  Board.background(bg_color);
  Board.endDraw();
  
}

void mark(){
 Board.beginDraw();
 Board.stroke(pencil);
 Board.endDraw();
  
}
 
void keyPressed(){
  //Eraser 
  if(keyCode==32){ 
  reset();
  
  }
  
}

void mousePressed(){
  if(mouseX>10 && mouseX<10+50 && mouseY>10 && mouseY<10+50 && state == "dashBoard"){
  state = "pad"; 
  println(state);
  
 } 
 
  else if(mouseX>10 && mouseX<10+50 && mouseY>10 && mouseY<10+50  && state == "pad"){
  state = "dashBoard"; 
  println(state);
  
  }
  
 if(mousePressed && mouseX>70 && mouseX<70+50 && mouseY>10 && mouseY<10+50 && state == "instructions"){
  state = "dashBoard";
  println("dash");
  delay(100);
  
 }
 
 if(mousePressed && mouseX>250 && mouseX<250+50 && mouseY>10 && mouseY<10+50 && state == "credits"){
   state = "dashBoard";
   delay(100);
 }
 
 if(mousePressed && mouseX>310 && mouseX<310+50 && mouseY>10 && mouseY<10+50 && state == "dashBoard"){
   println("exiting");
   exit();
   
 }
 
 if(mousePressed && mouseX>190 && mouseX<190+50 && mouseY>10 && mouseY<10+50 && state == "pencil"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>130 && mouseX<130+50 && mouseY>10 && mouseY<10+50 && state == "board"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>300 && mouseX<300+50 && mouseY>450 && mouseY<450+50 && state == "White"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>360 && mouseX<360+50 && mouseY>450 && mouseY<450+50 && state == "Black"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>420 && mouseX<420+50 && mouseY>450 && mouseY<450+50 && state == "Red"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>480 && mouseX<480+50 && mouseY>450 && mouseY<450+50 && state == "Green"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>540 && mouseX<540+50 && mouseY>450 && mouseY<450+50 && state == "Blue"){
   state = "dashBoard";
   delay(100);
   
 }
 
  if(mousePressed && mouseX>370 && mouseX<370+50 && mouseY>510 && mouseY<510+50 && state == "Small"){
   state = "dashBoard";
   delay(100);
   
 }
 
  if(mousePressed && mouseX>430 && mouseX<430+50 && mouseY>510 && mouseY<510+50 && state == "Medium"){
   state = "dashBoard";
   delay(100);
   
 }
 
  if(mousePressed && mouseX>490 && mouseX<490+50 && mouseY>510 && mouseY<510+50 && state == "Big"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>360 && mouseX<360+50 && mouseY>510 && mouseY<510+50 && state == "white"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>420 && mouseX<420+50 && mouseY>510 && mouseY<510+50 && state == "black"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>480 && mouseX<480+50 && mouseY>510 && mouseY<510+50 && state == "ReD"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>540 && mouseX<540+50 && mouseY>510 && mouseY<510+50 && state == "GreeN"){
   state = "dashBoard";
   delay(100);
   
 }
 
 if(mousePressed && mouseX>540 && mouseX<540+50 && mouseY>450 && mouseY<450+50 && state == "BluE"){
   state = "dashBoard";
   delay(100);
   
 }
 
}

void exit(){
  Board.save("Image.png");  
   
  super.exit(); 
  
 }
