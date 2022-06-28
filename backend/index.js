const express = require('express');
const cors = require('cors');
const path = require('path');
const nodemailer = require('nodemailer');
const { google } = require('googleapis');
//const estructs = require('../backend/structs/');
let json = require('../backend/usuarios.json');
let sync = 
//cargarCodigo

class usuario {
    constructor(_nombre, _apellido, _usuario, _correo, _numero, _contrasenia, _fechanac, _fechaReg) {
        this.nombre = _nombre;
        this.apellido = _apellido;
        this.usuario = _usuario;
        this.correo = _correo;
        this.numero = _numero;
        this.contrasenia = _contrasenia;
        this.fechaNac = _fechanac;
        this.fechaReg = _fechaReg;
        this.able = false;
        this.alta = false;
        this.type = 0;
    }
}

function sendMailCreated(mail, name, user, apellido) {
    contentHTML = `
    <div><p>Bienvenido al proyecto 2 del curso de manejo e implementación de archivos,
    su cuenta fue creada exitosamente, porfavor espere a que el administrador habilite su cuenta
    para poder acceder a su cuenta.</p></div><br>        
    <div>
        <h1>Información de la cuenta</h1><br>
        <ul>
            <li>Nombre: ${name}</li>
            <li>Apellido: ${apellido}</li>
            <li>Usuario: ${user}</li>
        </ul>
    </div>
    `;

    const CLIENTD_ID = "520082111374-kag96mv6ha5pvfq8lop9600sps0480m0.apps.googleusercontent.com";
    const CLIENT_SECRET = "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";
    const REDIRECT_URI = "https://developers.google.com/oauthplayground";
    const REFRESH_TOKEN = "1//04Iyi0XKrgtKKCgYIARAAGAQSNwF-L9IrpWkK5UBDonFcCYezoBrv8fuJf0U4utrmdh0DXPppGAIwWIp8sBs5jXL2KR8_RD-N5AQ";

    const oAuth2Client = new google.auth.OAuth2(
        CLIENTD_ID,
        CLIENT_SECRET,
        REDIRECT_URI
    );

    oAuth2Client.setCredentials({ refresh_token: REFRESH_TOKEN });

    async function sendMail() {
        try {
            const accessToken = await oAuth2Client.getAccessToken()
            const transporter = nodemailer.createTransport({
                service: "gmail",
                auth: {
                    type: "OAuth2",
                    user: "jonathanadm715@gmail.com",
                    clientId: CLIENTD_ID,
                    clientSecret: CLIENT_SECRET,
                    refreshToken: REFRESH_TOKEN,
                    accessToken: accessToken
                },
            });
            const mailOptions = {
                from: "<jonathanadm715@gmail.com>",
                to: mail,
                subject: "Creacion Usuario",
                html: contentHTML,
            };
            const result = await transporter.sendMail(mailOptions)
            return result;
        } catch (err) {
            console.log(err);
        }
    }
    sendMail()

}
const app = express();
app.use(cors());
app.use(express.json());

app.get('/', (req, res) => {
    res.send("Escuchando")
});


//Post del crear usuario
app.post('/CreateUser', (req, res) => {
    json = require('../backend/usuarios.json');
    let encontrado = -1;

    var datos = json; //leerJSON('usuarios.json');
    for (var i = 0; i < datos.length; i++) {
        if (req.body.usr == datos[i].usuario) {
            encontrado = i;
            break;
        }
    }

    if (encontrado >= 0) {
        res.json(new usuario('', '', '', '', 0, '', '', '',));
    } else {
        sendMailCreated(req.correo, req.nombre, req.usuario, req.apellido);
        datos.push(new usuario(req.body.nombre, req.body.apellido, req.body.usuario, req.body.correo, req.body.numero, req.body.contrasenia, req.body.fechaNac, req.body.fechaReg));
        var fs = require('fs');
        fs.writeFile("usuarios.json", JSON.stringify(datos), function (err) {
            if (err) throw err;
            console.log('complete');
        }

        );

        res.json(new usuario(req.nombre, req.apellido, req.usuario, req.correo, req.numero, req.contrasenia, req.fechaNac, req.fechaReg));
        console.log("LLEGA")

    }


});


//Post del login
app.post('/Login', (req, res) => {
    json = require('../backend/usuarios.json');

    //Aqui se busca en los usuarios del json
    let encontrado = -1;



    var datos = json; //leerJSON('usuarios.json');
    for (var i = 0; i < datos.length; i++) {
        if (req.body.usr == datos[i].correo) {
            encontrado = i;
            break;
        }
    }
    if (encontrado >= 0) {
        res.json(datos[i]);
    } else {
        res.json(new usuario('', '', '', '', 0, '', '', '',));
    }
});

//Post aqui se recupera la contrasenia
app.post('/RecuperarContrasenia', (req, res) => {

});


//Aqui se emvian los datos para la tabla de los usuarios que se tienen que dar de alta
app.post('/DarAlta', (req, res) => {
    console.log(req.body)

    json = require('../backend/usuarios.json');
    var datos = [];

    for (let i = 0; i < json.length; i++) {
        if (!json[i].alta) {
            datos.push(json[i])
        }
    }
    //res = datos;
    res.send(datos)
});


//Aqui se emvian los datos para la tabla de los usuarios que se tienen que habilitar
app.post('/Habilitar', (req, res) => {
    console.log(req.body)

    json = require('../backend/usuarios.json');
    var datos = [];

    for (let i = 0; i < json.length; i++) {
        if (!json[i].able) {
            datos.push(json[i])
        }
    }
    //res = datos;
    res.send(datos)
});

//Aqui se da de alta al usuario
app.post('/sendAlta', (req, res)=>{
    json = require('../backend/usuarios.json');
    for (let i = 0; i < json.length; i++) {
        if (req.body.usuario == json[i].usuario) {
            json[i].alta = true;
        }
    }


    var fs = require('fs');
    fs.writeFile("usuarios.json", JSON.stringify(json), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

app.post('/eliminar', (req, res)=>{
    json = require('../backend/usuarios.json');
    var datos = [];
    for (let i = 0; i < json.length; i++) {
        if (req.body.usuario != json[i].usuario) {
            datos.push(json[i]);
        }
    }

    json = datos;
    var fs = require('fs');
    fs.writeFile("usuarios.json", JSON.stringify(json), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

app.post('/HabilitarUsuario', (req, res)=>{
    json = require('../backend/usuarios.json');
    for (let i = 0; i < json.length; i++) {
        if (req.body.usuario == json[i].usuario) {
            json[i].able = true;
        }
    }


    var fs = require('fs');
    fs.writeFile("usuarios.json", JSON.stringify(json), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

//Metodo post para el syncronice
app.post('/Syncronize', (req, res) => {
    console.log(req.body)

    var datos = [];
    datos = require('../backend/Syncronice.json');
    //res = datos;
    res.send(datos)
});

function arbolRecursivo(datos, carpeta, propietario){}

app.post('/ChangeProperty',(req, res)=>{
console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivo(datos, req.body.nombreCarpeta, req.body.nuevoPropietario);
});

app.listen(3000, () => {
    console.log("Servidor en el puerto 3000")
});



