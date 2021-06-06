	node('master'){
    stage('Clean Workspace'){
        cleanWs()
    }

    stage('Fetch from Git'){
        // Correct branch name, if different from 'master'
        git branch: 'main', url:'https://github.com/tjsaaby/AMS-AB-Jenkins'
    }

    stage('Clean Build') {
        bat '"C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\AtmelStudio.exe" /clean RTOS_HH.atsln'
    }
    stage('Build') {
        bat '"C:\\Program Files (x86)\\Atmel\\Studio\\7.0\Vs\\AtmelStudio.exe" /build RTOS_HH.atsln'
    }
}
