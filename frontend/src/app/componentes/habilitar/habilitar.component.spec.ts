import { ComponentFixture, TestBed } from '@angular/core/testing';

import { HabilitarComponent } from './habilitar.component';

describe('HabilitarComponent', () => {
  let component: HabilitarComponent;
  let fixture: ComponentFixture<HabilitarComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ HabilitarComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(HabilitarComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
