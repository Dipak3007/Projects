const inputSlider = document.querySelector('[data-lengthSlider]')   //fetching using custom attribute
const lengthDisplay = document.querySelector('[data-length]');
const passwordDisplay = document.querySelector("[data-passwordDisplay]");
const copyMsg = document.querySelector("[data-copyMsg]");
const uppercaseCheck = document.querySelector("#uppercase");
const lowercaseCheck = document.querySelector("#lowercase");
const numbersCheck = document.querySelector("#numbers");
const symbolsCheck = document.querySelector("#symbols");
const indicator = document.querySelector("[data-indicator]");
const allCheckBox = document.querySelectorAll("input[type=checkbox]");

let password = "";
let passwordLength = 10;
let checkCount = 1;
uppercaseCheck.checked = true;  //one checkbox is checked initially

handleSlider();
setIndicator("#ccc");   //initial colour of indicator


// set password length and input slider background
function handleSlider() {
    inputSlider.value = passwordLength;
    lengthDisplay.innerText = passwordLength;
      const min = inputSlider.min;
      const max = inputSlider.max;
      inputSlider.style.backgroundSize = ((passwordLength - min) * 100) / (max - min) + "% 100%";
}

// handle input event on range slider
inputSlider.addEventListener('input', () => {
    passwordLength = inputSlider.value;
    handleSlider();
});

function setIndicator(color) {
    indicator.style.backgroundColor = color;
    indicator.style.boxShadow = `0px 0px 12px 1px ${color}`;
}

// genarate any random no. b/w min and max
function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

// generates any number b/w 0 - 9
function generateRandomNumber() {
    return getRndInteger(0, 9);
}

// generates any lowercase digit b/w a - z
function generateLowerCase() {
    return String.fromCharCode(getRndInteger(97, 122));
}

// generates any uppercase digit b/w A - Z
function generateUpperCase() {
    return String.fromCharCode(getRndInteger(65, 90));
}

//generates any symbols
function generateSymbol() {
    const symbols = '~`!@#$%^&*()_-+={[}]|:;"<,>.?/';
    const randIndx = getRndInteger(0, symbols.length);
    return symbols.charAt(randIndx);
}

function calcStrength() {
    let hasUpper = false;
    let hasLower = false;
    let hasNum = false;
    let hasSym = false;
    if (uppercaseCheck.checked) hasUpper = true;
    if (lowercaseCheck.checked) hasLower = true;
    if (numbersCheck.checked) hasNum = true;
    if (symbolsCheck.checked) hasSym = true;

    if (hasUpper && hasLower && (hasNum || hasSym) && passwordLength >= 8) setIndicator("#0f0");
    else if ((hasLower || hasUpper) && (hasNum || hasSym) && passwordLength >= 6) setIndicator("#ff0");
    else setIndicator("#f00");
}

async function copyContent() {
    if (passwordDisplay.value) {
        try {
            await navigator.clipboard.writeText(passwordDisplay.value);
            copyMsg.innerText = "Copied";
        }
        catch (err) {
            copyMsg.innerText = "Failed";
        }

        copyMsg.classList.add("active");
        setTimeout(() => {
            copyMsg.classList.remove("active");
        }, 2000);
    }
}

// Handle generate password
function generatePass() {
    // none of the checkboxes are selected
    if (checkCount <= 0) return;

    // password-length should be >= selected no. of checkbox
    if (passwordLength < checkCount) {
        passwordLength = checkCount;
        handleSlider();
    }

    // remove the previous password
    if (password.length) password = "";

    // add selected checkbox functions to an array
    let funcArr = [];
    if (uppercaseCheck.checked) funcArr.push(generateUpperCase);
    if (lowercaseCheck.checked) funcArr.push(generateLowerCase);
    if (numbersCheck.checked) funcArr.push(generateRandomNumber);
    if (symbolsCheck.checked) funcArr.push(generateSymbol);

    //all conditions must be fulfilled, so each function should be called once
    for (let i = 0; i < funcArr.length; i++)
        password += funcArr[i]();

    for (let i = 0; i < passwordLength - funcArr.length; i++) {
        let randIndx = getRndInteger(0, funcArr.length - 1);   //random checked option
        password += funcArr[randIndx]();
    }

    password = shuffleArray(Array.from(password));
    passwordDisplay.value = password;
    calcStrength();
}

// handle check-count and password-length (password-length >= check-count)
allCheckBox.forEach((checkbox) => {
    checkbox.addEventListener('change', handleCheckBoxChange);
    if (passwordLength < checkCount) {
        passwordLength = checkCount;
        handleSlider();
    }
})

function handleCheckBoxChange() {
    checkCount = 0;
    allCheckBox.forEach((checkbox) => {
        if (checkbox.checked)
            checkCount += 1
    })
}

// Shuffle the array randomly - Fisher Yates Method
function shuffleArray(array) {
    for (let i = array.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        const temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    let str = "";
    array.forEach((el) => (str += el));
    return str;
}