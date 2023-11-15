const userTab=document.querySelector('[data-userWeather]')
const searchTab=document.querySelector('[data-searchWeather]')
const userContainer=document.querySelector('.weather-container')
const grantAccessContainer = document.querySelector('.grant-location-container');
const searchForm = document.querySelector('[data-searchForm]');
const loadingScreen = document.querySelector('.loading-container');
const userInfoContainer = document.querySelector('.user-info-container');
const errorScreen=document.querySelector('.error-container');

let currTab=userTab;
const API_KEY = "d1845658f92b31c64bd94f06f7188c9c";
currTab.classList.add('current-tab')
getFromSessionStorage()

userTab.addEventListener('click',()=>{
    switchTab(userTab);
})

searchTab.addEventListener('click',()=>{
    switchTab(searchTab);
})

function switchTab(clickedTab){
    errorScreen.classList.remove('active');
    if(clickedTab!=currTab){
        currTab.classList.remove('current-tab');
        currTab=clickedTab
        currTab.classList.add('current-tab');

        if(!searchForm.classList.contains('active')){
            userInfoContainer.classList.remove('active');
            grantAccessContainer.classList.remove('active');
            searchForm.classList.add('active');
        }
        else{
            searchForm.classList.remove('active');
            userInfoContainer.classList.remove('active');
            getFromSessionStorage();
        }
    }
}

//check if coordinates are already present in session storage
function getFromSessionStorage(){
    const loc=sessionStorage.getItem('user-loc');
    if(!loc){
        grantAccessContainer.classList.add('active');
    }
    else{
        grantAccessContainer.classList.remove('active');
        const location=JSON.parse(loc);
        fetchUserWeatherInfo(location);
    }
}

async function fetchUserWeatherInfo(location){
    const {lat, lon} = location;
    loadingScreen.classList.add('active');
    errorScreen.classList.remove('active');

    // API call
    try{
        const response=await fetch(`https://api.openweathermap.org/data/2.5/weather?lat=${lat}&lon=${lon}&appid=${API_KEY}&units=metric`);
    
        const data=await response.json();
        if(!data.sys)   throw data;
        loadingScreen.classList.remove('active')
        userInfoContainer.classList.add('active')
        renderWeatherInfo(data)
    }
    catch(e){
        loadingScreen.classList.remove("active");
        errorScreen.classList.add('active');
    }   
}

function renderWeatherInfo(data){
    const cityName=document.querySelector('[data-cityName]')
    const countryIcon=document.querySelector('[data-countryIcon]')
    const desc=document.querySelector('[data-weatherDesc]')
    const weatherIcon=document.querySelector('[data-weatherIcon]')
    const temp=document.querySelector('[data-temp]')
    const windspeed=document.querySelector('[data-windspeed]')
    const humidity=document.querySelector('[data-humidity]')
    const cloud=document.querySelector('[data-cloud]')

    cityName.innerText=data?.name;
    countryIcon.src=`https://flagcdn.com/144x108/${data?.sys?.country.toLowerCase()}.png`;
    desc.innerText = data?.weather?.[0]?.description;
    weatherIcon.src = `http://openweathermap.org/img/w/${data?.weather?.[0]?.icon}.png`;
    temp.innerText = `${data?.main?.temp} °C`;
    windspeed.innerText = `${data?.wind?.speed} m/s`;
    humidity.innerText = `${data?.main?.humidity}%`;
    cloud.innerText = `${data?.clouds?.all}%`;
}

const grantAccessButton = document.querySelector("[data-grantAccess]");
grantAccessButton.addEventListener("click", getLocation);

function getLocation(){
    grantAccessContainer.classList.remove('active')
    if(navigator.geolocation)
        navigator.geolocation.getCurrentPosition(showPosition)
    else{
        
    }
}

function showPosition(position){
    const userCoordinates={
        lat: position.coords.latitude,
        lon: position.coords.longitude,
    }
    sessionStorage.setItem("user-loc", JSON.stringify(userCoordinates));
    fetchUserWeatherInfo(userCoordinates);
}

const searchInput = document.querySelector("[data-searchInput]");
function getCityInfo(){
    let cityName = searchInput.value;
    if(cityName === "")
        return;
    else 
        fetchSearchWeatherInfo(cityName);
}

async function fetchSearchWeatherInfo(city){
    loadingScreen.classList.add('active');
    userInfoContainer.classList.remove('active');
    grantAccessContainer.classList.remove('active');
    errorScreen.classList.remove('active');
    try{
        const response = await fetch(
            `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${API_KEY}&units=metric`
          );
        const data = await response.json();
        if(!data.sys)   throw data;
        loadingScreen.classList.remove("active");
        userInfoContainer.classList.add("active");
        renderWeatherInfo(data);
    }
    catch(e){
        loadingScreen.classList.remove("active");
        errorScreen.classList.add('active');
    }
}