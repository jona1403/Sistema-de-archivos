const express = require('express');
const cors = require('cors');
const path = require('path');
const nodemailer = require('nodemailer');
const { google } = require('googleapis');
//const estructs = require('../backend/structs/');
let json = require('../backend/usuarios.json');
const { Console } = require('console');
const { arch } = require('os');
let archivosUsuario = []
let archivosColaboracion = []

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
        this.able = true;
        this.alta = false;
        this.type = 0;
    }
}


function Mail(mail, content, subject) {
    const CLIENTD_ID = "469774724828-hfklhmo78n75sha8lqvkqmd09u9m2hdr.apps.googleusercontent.com";
    const CLIENT_SECRET = "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";
    const REDIRECT_URI = "https://developers.google.com/oauthplayground";
    const REFRESH_TOKEN = "1//04MX_hZ-Fe53CCgYIARAAGAQSNwF-L9IrJOSfKVQGeoBK3bCduins_vQRW3CsI9HMtItdMr4tuxsBq0owLansvnKrOr7691P3zBw";

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
                subject: subject,
                html: content,
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
        if (req.body.usuario == datos[i].usuario) {
            encontrado = i;
            break;
        }
    }

    if (encontrado >= 0) {
        res.json(new usuario('', '', '', '', 0, '', '', '',));
    } else {
        console.log(req.body.correo)

        const contenHTML = `
        <h1>Datos del Usuario</h1>
        <ul>
            <li>Nombre: ${req.body.nombre}</li>
            <li>Apellido: ${req.body.apellido}</li>
            <li>Usuario: ${req.body.usuario}</li>
            <li>Fecha y hora: ${req.body.fechaReg}</li>
        </ul>
        <p>Por favor espere a que el administrador de de alta su cuenta</p>
        `;

        Mail(req.body.correo, contenHTML, "Creacion Usuario");
        //sendMailCreated(req.body.correo, req.body.nombre, req.body.usuario,req.body.apellido);


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

app.post('/UpdateUser', (req, res) => {
    console.log(req.body.usuario)
    json = require('../backend/usuarios.json');
    let encontrado = -1;

    var datos = json; //leerJSON('usuarios.json');
    for (var i = 0; i < datos.length; i++) {
        if (req.body.correo == datos[i].correo) {

            datos[i].nombre = req.body.nombre;
            datos[i].apellido = req.body.apellido;
            datos[i].usuario = req.body.usuario;
            datos[i].numero = req.body.numero;
            datos[i].contrasenia = req.body.contrasenia;
            var fs = require('fs');
            fs.writeFile("usuarios.json", JSON.stringify(datos), function (err) {
                if (err) throw err;
                console.log('complete');
            }

            );
        }
    }

    res.json({
        user: 'Correct'
    });
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
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for(var i = 0; i < 8; i++){
        text += possible.charAt(Math.floor(Math.random()*possible.length));
    }

    const contenHTML = `
    <h1>Recuperacion de contraseña</h1>
    <p>El codigo para recuperacion de cuenta es: </p><br>
    <h2><b>${text}</b></h2>
    `;

    Mail(req.body.correo, contenHTML, "Recupeacion de contraseña");



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
app.post('/sendAlta', (req, res) => {
    json = require('../backend/usuarios.json');
    for (let i = 0; i < json.length; i++) {
        if (req.body.usuario == json[i].usuario) {
            json[i].alta = true;
        }
    }
    const contenHTML = `
    <h1>Su cuenta ha sido dada de alta, ahora podra ingresar a FuBox.</h1>

    `;

    Mail(req.body.correo, contenHTML, "Cuenta dada de alta");

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

app.post('/eliminar', (req, res) => {
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

app.post('/HabilitarUsuario', (req, res) => {
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

function recursivoProp(datos, propietario) {
    for (var i = 0; i < datos.length; i++) {

        datos[i].propietario = propietario;
        if (datos[i].tipo == 0) {
            datos[i].contenido = recursivoProp(datos[i].contenido, propietario)
        }
    }
    return datos;
}

function arbolRecursivo(datos, carpeta, propietario) {
    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == carpeta) {
            datos[i].propietario = propietario;
            if (datos[i].tipo == 0) {
                datos[i].contenido = recursivoProp(datos[i].contenido, propietario)
            }
            return datos;
        }
        if (datos[i].tipo == 0) {
            datos[i].contenido = arbolRecursivo(datos[i].contenido, carpeta, propietario)
        }
    }
    return datos;
}

app.post('/ChangeProperty', (req, res) => {
    console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivo(datos, req.body.nombreCarpeta, req.body.nuevoPropietario);

    var fs = require('fs');
    fs.writeFile("Syncronice.json", JSON.stringify(datos), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

function recursivoPropG(datos, propietario) {
    for (var i = 0; i < datos.length; i++) {

        datos[i].grupo.push(propietario);
        if (datos[i].tipo == 0) {
            datos[i].contenido = recursivoPropG(datos[i].contenido, propietario)
        }
    }
    return datos;
}

function arbolRecursivoG(datos, carpeta, propietario) {
    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == carpeta) {


            const contenHTML = `
            <h1>Colaborador de Carpeta</h1>
            <p>Ahora es colaborador de la carpeta <b>${datos[i].nombre}</b></p>
            `;
            var correo;
            var usuarios = require('../backend/usuarios.json');
            for(var q = 0; q < usuarios.length; q++){
                if(propietario == usuarios[q].usuario){
                    correo = usuarios[q].correo;
                    break;
                }
            }

            Mail(correo, contenHTML, "Colaborador de carpeta");



            datos[i].grupo.push(propietario);
            if (datos[i].tipo == 0) {
                datos[i].contenido = recursivoPropG(datos[i].contenido, propietario)
            }
            return datos;
        }
        if (datos[i].tipo == 0) {
            datos[i].contenido = arbolRecursivoG(datos[i].contenido, carpeta, propietario)
        }
    }
    return datos;
}

app.post('/addColaborador', (req, res) => {
    console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivoG(datos, req.body.nombreCarpeta, req.body.nuevoPropietario);

    var fs = require('fs');
    fs.writeFile("Syncronice.json", JSON.stringify(datos), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

app.post('/Dishable', (req, res) => {
    json = require('../backend/usuarios.json');
    for (let i = 0; i < json.length; i++) {
        if (req.body.usr == json[i].correo) {
            json[i].able = false;
        }
    }

    const contenHTML = `
    <p>Su usuario ha sido Inhabilitado, por favor contactese con el administrador.</p>
    `;

    Mail(req.body.usr, contenHTML, "Inhabilitación de cuenta");

    const contenHTML2 = `
    <h1>Datos del Usuario</h1>
    <ul>
        <li>Nombre: ${req.body.usr}</li>
    </ul>
    <p>El usuario ha sido inhabilitado por ingresar una contraseña incorrecta por 3 veces</p>
    `;

    Mail('jonathanadm715@gmail.com', contenHTML2, "Cuenta Inhabilitada");

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


function arbolUsuario(datos, propietario) {
    for (var i = 0; i < datos.length; i++) {
        if (datos[i].propietario == propietario) {
            archivosUsuario.push(datos[i])
            //return datos;
        } else {
            if (datos[i].tipo == 0) {
                arbolUsuario(datos[i].contenido, propietario)
                //datos[i].contenido = arbolRecursivo(datos[i].contenido, carpeta, propietario)
            }
        }
    }
}

app.post('/archivosUsuario', (req, res) => {
    console.log(req.body.usuario)

    var datos = [];
    datos = require('../backend/Syncronice.json');
    arbolUsuario(datos, req.body.usuario);
    //res = datos;
    res.send(archivosUsuario)
    archivosUsuario = []
});


function arbolColab(datos, propietario) {
    bandera = false;
    for (var i = 0; i < datos.length; i++) {

        for (var j = 0; j < datos[i].grupo.length; j++) {
            if (datos[i].grupo[j] == propietario) {
                bandera = true;
                break;
            }
        }
        if (bandera) {
            console.log('Entra')
            console.log(datos[i].nombre)
            archivosColaboracion.push(datos[i])
            bandera = false;
            //return datos;
        } else {
            if (datos[i].tipo == 0) {
                arbolColab(datos[i].contenido, propietario)
                //datos[i].contenido = arbolRecursivo(datos[i].contenido, carpeta, propietario)
            }
        }
    }
}

app.post('/ArchivosColavoracion', (req, res) => {
    console.log(req.body.usuario)

    var datos = [];
    datos = require('../backend/Syncronice.json');
    arbolColab(datos, req.body.usuario);
    //res = datos;
    res.send(archivosColaboracion)
    archivosColaboracion = []
});


function arbolRecursivoDelFichero(datos, carpeta) {
    var datos2 = [];
    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == carpeta) {

        } else {
            if (datos[i].tipo == 0) {
                datos[i].contenido = arbolRecursivoDelFichero(datos[i].contenido, carpeta)
            }
            datos2.push(datos[i])
        }

    }
    return datos2;
}


app.post('/DeleteFichero', (req, res) => {
    console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivoDelFichero(datos, req.body.nombreCarpeta);

    var fs = require('fs');
    fs.writeFile("Syncronice.json", JSON.stringify(datos), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});



function arbolRecursivoUpdateCarpet(datos, carpeta, nuevo) {
    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == carpeta) {
            
            const contenHTML = `
            <h1>Edicion Carpeta</h1>
            <p>Se ha cambiado el nombre de la carpeta <b>${datos[i].nombre}</b> a <b>${nuevo}</b>. </p>
            `;
            datos[i].nombre = nuevo;
            var correo;
            var usuarios = require('../backend/usuarios.json');
            for(var q = 0; q < usuarios.length; q++){
                if(datos[i].propietario == usuarios[q].usuario){
                    correo = usuarios[q].correo;
                    break;
                }
            }

            Mail(correo, contenHTML, "Edicion Carpetas");

            return datos;
        } else {
            if (datos[i].tipo == 0) {
                datos[i].contenido = arbolRecursivoUpdateCarpet(datos[i].contenido, carpeta, nuevo)
            }
        }

    }
    return datos;
}


app.post('/UUUpdateCarpet', (req, res) => {
    console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivoUpdateCarpet(datos, req.body.nombreCarpeta, req.body.nuevoPropietario);

    var fs = require('fs');
    fs.writeFile("Syncronice.json", JSON.stringify(datos), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});




function arbolRecursivoAddFicheros(datos, carpeta, tipo, nombre) {

    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == nombre) {
            if (datos[i].tipo == 0) {
                var datoss;
                if (tipo == 0) {
                    datoss = { nombre: carpeta, propietario: datos[i].propietario, tipo: tipo, grupo: datos[i].grupo, contenido: [] }
                } else {
                    datoss = { nombre: carpeta, propietario: datos[i].propietario, tipo: tipo, grupo: datos[i].grupo, texto: "" }
                }
                datos[i].contenido.push(datoss)
            }
        } else {
            if (datos[i].tipo == 0) {
                datos[i].contenido = arbolRecursivoAddFicheros(datos[i].contenido, carpeta, tipo, nombre)
            }
        }

    }
    return datos;
}


app.post('/AddFichero', (req, res) => {
    console.log(req.body)
    var datos = [];
    datos = require('../backend/Syncronice.json');
    datos = arbolRecursivoAddFicheros(datos, req.body.nombreCarpeta, req.body.nivel, req.body.nuevoPropietario);

    var fs = require('fs');
    fs.writeFile("Syncronice.json", JSON.stringify(datos), function (err) {
        if (err) throw err;
        console.log('complete');
    }

    );
    res.json({
        user: 'Correct'
    });
});

function retArchivo(datos, nombre){

    for (var i = 0; i < datos.length; i++) {
        if (datos[i].nombre == nombre) {
            return datos[i];
        } else {
            if (datos[i].tipo == 0) {
                return retArchivo( datos[i].contenido, nombre)
            }
        }

    }
    return null;
}

//Retorna el archivo a editar
app.post('/ModifyFile', (req, res) => {
    console.log(req.body)
    var datos = [];
    var archivo;
    datos = require('../backend/Syncronice.json');
    archivo = retArchivo( datos, req.body.nombreCarpeta);
    console.log(archivo)
    res.send(archivo)
});


app.listen(3000, () => {
    console.log("Servidor en el puerto 3000")
});



