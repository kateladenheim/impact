"use strict";

const server = io("https://impact.mehtank.com");
const options = ["It was shocking or alarming", "It was exciting or athletic", "It was impressive or inspiring", "It was lame or comical"];

server.on("connect", function () {
  console.log("Connected to impact server");
});

server.on("fall", function (data) {
  console.log("Fall noted:", data);

  var timeleft = 9;
  var downloadTimer = setInterval(function () {
    if (timeleft <= 0) {
      clearInterval(downloadTimer);
      document.getElementById("countdown").innerHTML = "0 seconds left to vote";
    } else {
      document.getElementById("countdown").innerHTML =
        timeleft + " seconds left to vote";
    }
    timeleft -= 1;
  }, 1000);

  var elem = document.getElementById("content");
  elem.innerHTML = "";

  for (let opt of options) {
    var button = document.createElement("button");
    button.id = "vote" + opt;
    button.className = "votebutton vote" + opt;
    button.innerHTML = "+ " + opt;
    button.onclick = function () {
      console.log("voting for " + opt);
      server.emit("vote", opt);
    };
    elem.appendChild(button);
  }
});

server.on("closed", function (data) {
  console.log("Voting closed");

  var elem = document.getElementById("content");
  elem.innerHTML = "Voting closed";
});

server.on("results", function (data) {
  console.log("Votes tallied:", data);

  var elem = document.getElementById("content");
  elem.innerHTML = "";

  for (let opt in data) {
    var vote = document.createElement("div");
    // vote.innerHTML = "Option " + opt + " got " + data[opt] + " votes.";
    vote.innerHTML = "compiling...";
    elem.appendChild(vote);
  }
});

server.on("ack", function (data) {
  console.log("Acknowledged vote for: ", data);
  for (let elem of document.querySelectorAll("button")) {
    elem.style.backgroundColor = "gray";
    elem.style.border = "none";
  }
  var elem = document.getElementById("vote" + data);
  elem.style.backgroundColor = "black";
  elem.style.fontSize = "24px";
});
