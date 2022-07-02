import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EditarArchivosComponent } from './editar-archivos.component';

describe('EditarArchivosComponent', () => {
  let component: EditarArchivosComponent;
  let fixture: ComponentFixture<EditarArchivosComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EditarArchivosComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(EditarArchivosComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
