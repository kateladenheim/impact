
var arr = [];
var choice = document.querySelector('[name="selection"]:checked')
if (choice != null) {
    arr.push(choice.value);
}

console.log(arr);

// this is the timer

var timeleft = 10;
var downloadTimer = setInterval(function () {

    if (timeleft <= 0) {
        clearInterval(downloadTimer);
        document.getElementById("countdown").innerHTML = "Finished";
        // document.getElementById('vote').submit();
        // window.open('/index.html',"_self");

        function mostOccurringElement(arr) {
            var max = arr[0],
                counter = {},
                i = arr.length,
                element;
        
            while (i--) {
                element = arr[i];
                if (!counter[element]) counter[element] = 0;
                counter[element]++;
                if (counter[max] < counter[element]) max = element;
            }
        
            return max;
        }
        
        console.log(max);

    } else {
        document.getElementById("countdown").innerHTML = timeleft + " seconds left to vote";
    }
    timeleft -= 1;
}, 1000);