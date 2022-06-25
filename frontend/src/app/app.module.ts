import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './componentes/login/login.component';
import { CrearUsuarioComponent } from './componentes/crear-usuario/crear-usuario.component';
import { OlvideContraseniaComponent } from './componentes/olvide-contrasenia/olvide-contrasenia.component';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { InicioAdminComponent } from './componentes/inicio-admin/inicio-admin.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    CrearUsuarioComponent,
    OlvideContraseniaComponent,
    InicioComponent,
    InicioAdminComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    ReactiveFormsModule,
    FormsModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
