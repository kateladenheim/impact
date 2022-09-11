const candidates = [
    {
        first: "it was shocking or alarming",
        votes: 0
    },
    {
        first: "it was impressive or inspiring",
        votes: 0
    },
    {
        first: "it was lame or underwhelming",
        votes: 0
    },
    {
        first: "it was easeful or smooth",
        votes: 0
    },
];

//create a copy of the array for sorting the winners
var winningCandidates = candidates.slice();
var button;
var raceOver = false;

var container = document.querySelector(".votingContainer");
const winningContainer = document.querySelector(".winning");

//initial call to display the votes
displayVotes();

// countdown 

var timeleft = 10;
var downloadTimer = setInterval(function () {

    if (timeleft <= 0) {
        clearInterval(downloadTimer);
        document.getElementById("countdown").innerHTML = "Finished";
        
        var winner = winningCandidates.sort(function (a, b) {
            const aFirst = a.votes;
            const bFirst = b.votes;
            return aFirst < bFirst ? 1 : -1;
        });

        var name = winner[0].first;

        wonRace(name);
    } else {
        document.getElementById("countdown").innerHTML = timeleft + " seconds left to vote";
    }
    timeleft -= 1;
}, 1000);

function sendVote(x) {
    if (raceOver === true) {
        return;
    }

    
    for (var i = candidates.length - 1; i >= 0; i--) {
        var candidate = candidates[i];

        //if index of button equals index of candidate
        if (x === i) {
            candidate.votes++;

            displayVotes();
            var currentVote = document.querySelector(`.votingContainer .singleCandidate:nth-child(${i + 1})`);
            //currentVote.classList.add(`${candidate.votes}`);
            // if (candidate.votes === 5) {
            // if (timeleft = 0)
            //   var name = candidate.first;
            //   wonRace(name);
            //   return;
        }
        winningRace();
    }
}

function displayVotes() {
    if (raceOver === true) {
        return;
    }
    //build the HTML
    var html = candidates
        .map(candidate => {
            var name = candidate.first;
            var votes = candidate.votes;
            return `<div class="singleCandidate vote${candidate.votes}">
        <div class="name">${name}</div>
        <div class="votes">${votes}</div>
        <button>Vote +</button>
      </div>
    `;
        })
        .join("");

    //display it
    container.innerHTML = html;


    //add an event listener to all the buttons
    button = document.querySelectorAll("button");
    for (var x = 0; x < button.length; x++) {
        //when clicked, send to function with index of the clicked button
        button[x].addEventListener("click", sendVote.bind(null, x));
    }
}

function winningRace() {
    if (raceOver === true) {
        return;
    }
    var first = winningCandidates.sort(function (a, b) {
        const aFirst = a.votes;
        const bFirst = b.votes;
        return aFirst < bFirst ? 1 : -1;
    });

    var firstName = first[0].first;
    var firstVotes = first[0].votes;

    winningContainer.innerHTML = `${firstName}`;
    if (first[0].votes == first[1].votes) {
        winningContainer.innerHTML = `tied`;
    }
}

function wonRace(name) {
    raceOver = true;
    document.querySelector('body').classList.add("votingDone");
    winningContainer.innerHTML = `${name} — crowd compiling.`;
    //button = document.querySelectorAll("button");
    for (var x = button.length - 1; x >= 0; x--) {
        button[x].removeEventListener("click", sendVote.bind(null, x));
    }
}